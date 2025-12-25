/**
  ******************************************************************************
  * @file    WIZnet MDK5 Project Template  ../main.c 
  * @author  wr
  * @version V1.0.0
  * @date    2021-11-22
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 WIZnet H.K. Ltd.</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "bsp_socket.h"

#define SOCK_ANY_PORT_NUM  0xC000;

static uint16_t sock_any_port = SOCK_ANY_PORT_NUM;
static uint16_t sock_io_mode[2] = {0};
static uint16_t sock_is_sending[2] = {0};
static uint16_t sock_remained_size[2][_WIZCHIP_SOCK_NUM_] = {0};
static uint8_t  sock_pack_info[2][_WIZCHIP_SOCK_NUM_] = {0};

#if _WIZCHIP_ == 5200
   static uint16_t sock_next_rd[2][_WIZCHIP_SOCK_NUM_] ={0};
#endif

#define CHECK_SOCKNUM(chipNum,sn)   \
   do{                    \
      if(sn > _WIZCHIP_SOCK_NUM_) return SOCKERR_SOCKNUM;   \
   }while(0);             \

#define CHECK_SOCKMODE(chipNum,mode)  \
   do{                     \
      if((getSn_MR(chipNum, sn) & 0x0F) != mode) return SOCKERR_SOCKMODE;  \
   }while(0);              \

#define CHECK_SOCKINIT(chipNum)   \
   do{                     \
      if((getSn_SR(chipNum,sn) != SOCK_INIT)) return SOCKERR_SOCKINIT; \
   }while(0);              \

#define CHECK_SOCKDATA(chipNum)   \
   do{                     \
      if(len == 0) return SOCKERR_DATALEN;   \
   }while(0);              \



int8_t socket(uint8_t chipNum, uint8_t sn, uint8_t protocol, uint16_t port, uint8_t flag)
{
	CHECK_SOCKNUM(chipNum,sn);
	switch(protocol)
	{
      case Sn_MR_TCP :
      case Sn_MR_UDP :
      case Sn_MR_MACRAW :
         break;
   #if ( _WIZCHIP_ < 5200 )
      case Sn_MR_IPRAW :
      case Sn_MR_PPPoE :
         break;
   #endif
      default :
         return SOCKERR_SOCKMODE;
	}
	if((flag & 0x06) != 0) return SOCKERR_SOCKFLAG;
#if _WIZCHIP_ == 5200
   if(flag & 0x10) return SOCKERR_SOCKFLAG;
#endif
	   
	if(flag != 0)
	{
   	switch(protocol)
   	{
   	   case Sn_MR_TCP:
   	      if((flag & (SF_TCP_NODELAY|SF_IO_NONBLOCK))==0) return SOCKERR_SOCKFLAG;
   	      break;
   	   case Sn_MR_UDP:
   	      if(flag & SF_IGMP_VER2)
   	      {
   	         if((flag & SF_MULTI_ENABLE)==0) return SOCKERR_SOCKFLAG;
   	      }
   	      #if _WIZCHIP_ == 5500
      	      if(flag & SF_UNI_BLOCK)
      	      {
      	         if((flag & SF_MULTI_ENABLE) == 0) return SOCKERR_SOCKFLAG;
      	      }
   	      #endif
   	      break;
   	   default:
   	      break;
   	}
   }
	close(chipNum,sn);
	setSn_MR(chipNum,sn, (protocol | (flag & 0xF0)));
	if(!port)
	{
	   port = sock_any_port++;
	   if(sock_any_port == 0xFFF0) sock_any_port = SOCK_ANY_PORT_NUM;
	}
   setSn_PORT(chipNum,sn,port);	
   setSn_CR(chipNum,sn,Sn_CR_OPEN);
   while(getSn_CR(chipNum,sn));
	sock_io_mode[chipNum] |= ((flag & SF_IO_NONBLOCK) << sn);   
   sock_is_sending[chipNum] &= ~(1<<sn);
   sock_remained_size[chipNum][sn] = 0;
   sock_pack_info[chipNum][sn] = 0;
   while(getSn_SR(chipNum,sn) == SOCK_CLOSED);
   return (int8_t)sn;
}	   

int8_t close(uint8_t chipNum, uint8_t sn)
{
	CHECK_SOCKNUM(chipNum,sn);
	
	setSn_CR(chipNum,sn,Sn_CR_CLOSE);
   /* wait to process the command... */
	while( getSn_CR(chipNum,sn) );
	/* clear all interrupt of the socket. */
	setSn_IR(chipNum,sn, 0xFF);
	sock_is_sending[chipNum] &= ~(1<<sn);
	sock_remained_size[chipNum][sn] = 0;
	sock_pack_info[chipNum][sn] = 0;
	while(getSn_SR(chipNum,sn) != SOCK_CLOSED);
	return SOCK_OK;
}

int8_t listen(uint8_t chipNum, uint8_t sn)
{
	CHECK_SOCKNUM(chipNum,sn);
	CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
	CHECK_SOCKINIT(chipNum);
	setSn_CR(chipNum,sn,Sn_CR_LISTEN);
	while(getSn_CR(chipNum,sn)){};
   while(getSn_SR(chipNum,sn) != SOCK_LISTEN)
   {
      if(getSn_CR(chipNum,sn) == SOCK_CLOSED)
      {
         close(chipNum,sn);
         return SOCKERR_SOCKCLOSED;
      }
   }
   return SOCK_OK;
}


int8_t connect(uint8_t chipNum, uint8_t sn, uint8_t * addr, uint16_t port)
{
   CHECK_SOCKNUM(chipNum,sn);
   CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
   CHECK_SOCKINIT(chipNum);
   //M20140501 : For avoiding fatal error on memory align mismatched
   //if( *((uint32_t*)addr) == 0xFFFFFFFF || *((uint32_t*)addr) == 0) return SOCKERR_IPINVALID;
   {
      uint32_t taddr;
      taddr = ((uint32_t)addr[0] & 0x000000FF);
      taddr = (taddr << 8) + ((uint32_t)addr[1] & 0x000000FF);
      taddr = (taddr << 8) + ((uint32_t)addr[2] & 0x000000FF);
      taddr = (taddr << 8) + ((uint32_t)addr[0] & 0x000000FF);
      if( taddr == 0xFFFFFFFF || taddr == 0) return SOCKERR_IPINVALID;
   }
   //
	
	if(port == 0) return SOCKERR_PORTZERO;
	setSn_DIPR(chipNum,sn,addr);
	setSn_DPORT(chipNum,sn,port);
   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR(0);
   #endif
	setSn_CR(chipNum,sn,Sn_CR_CONNECT);
   while(getSn_CR(chipNum,sn));
   if(sock_io_mode[chipNum] & (1<<sn)) return SOCK_BUSY;
   #ifndef NO_CONNECT_WAIT
   while(getSn_SR(chipNum,sn) != SOCK_ESTABLISHED)
   {   
		if (getSn_IR(chipNum,sn) & Sn_IR_TIMEOUT)
		{
			setSn_IR(chipNum,sn, Sn_IR_TIMEOUT);
         #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
            setSUBR(chipNum,(uint8_t*)"\x00\x00\x00\x00");
         #endif
         return SOCKERR_TIMEOUT;
		}
	}
	#endif
   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR(chipNum,(uint8_t*)"\x00\x00\x00\x00");
   #endif
   
   return SOCK_OK;
}

int8_t disconnect(uint8_t chipNum, uint8_t sn)
{
   CHECK_SOCKNUM(chipNum,sn);
   CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
	setSn_CR(chipNum,sn,Sn_CR_DISCON);
	/* wait to process the command... */
	while(getSn_CR(chipNum,sn));
	sock_is_sending[chipNum] &= ~(1<<sn);
   if(sock_io_mode[chipNum] & (1<<sn)) return SOCK_BUSY;
	while(getSn_SR(chipNum,sn) != SOCK_CLOSED)
	{
	   if(getSn_IR(chipNum,sn) & Sn_IR_TIMEOUT)
	   {
	      close(chipNum,sn);
	      return SOCKERR_TIMEOUT;
	   }
	}
	return SOCK_OK;
}

int32_t send(uint8_t chipNum, uint8_t sn, uint8_t * buf, uint16_t len)
{
   uint8_t tmp=0;
   uint16_t freesize=0;
   
   CHECK_SOCKNUM(chipNum,sn);
   CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
   CHECK_SOCKDATA(chipNum);
   tmp = getSn_SR(chipNum,sn);
   if(tmp != SOCK_ESTABLISHED && tmp != SOCK_CLOSE_WAIT) return SOCKERR_SOCKSTATUS;
   if( sock_is_sending[chipNum] & (1<<sn) )
   {
      tmp = getSn_IR(chipNum,sn);
      if(tmp & Sn_IR_SENDOK)
      {
         setSn_IR(chipNum,sn, Sn_IR_SENDOK);
         #if _WZICHIP_ == 5200
            if(getSn_TX_RD(chipNum,sn) != sock_next_rd[chipNum][sn])
            {
               setSn_CR(chipNum,sn,Sn_CR_SEND);
               while(getSn_CR(chipNum,sn));
               return SOCKERR_BUSY;
            }
         #endif
         sock_is_sending[chipNum] &= ~(1<<sn);         
      }
      else if(tmp & Sn_IR_TIMEOUT)
      {
         close(chipNum,sn);
         return SOCKERR_TIMEOUT;
      }
      else return SOCK_BUSY;
   }
   freesize = getSn_TxMAX(chipNum,sn);
   if (len > freesize) len = freesize; // check size not to exceed MAX size.
   while(1)
   {
      freesize = getSn_TX_FSR(chipNum,sn);
      tmp = getSn_SR(chipNum,sn);
      if ((tmp != SOCK_ESTABLISHED) && (tmp != SOCK_CLOSE_WAIT))
      {
         close(chipNum,sn);
         return SOCKERR_SOCKSTATUS;
      }
      if( (sock_io_mode[chipNum] & (1<<sn)) && (len > freesize) ) return SOCK_BUSY;
      if(len <= freesize) break;
   }
   wiz_send_data(chipNum,sn, buf, len);
   #if _WIZCHIP_ == 5200
      sock_next_rd[chipNum][sn] = getSn_TX_RD(chipNum,sn) + len;
   #endif
   setSn_CR(chipNum,sn,Sn_CR_SEND);
   /* wait to process the command... */
   while(getSn_CR(chipNum,sn));
   sock_is_sending[chipNum] |= (1 << sn);
   return len;
}


int32_t recv(uint8_t chipNum, uint8_t sn, uint8_t * buf, uint16_t len)
{
   uint8_t  tmp = 0;
   uint16_t recvsize = 0;
   CHECK_SOCKNUM(chipNum,sn);
   CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
   CHECK_SOCKDATA(chipNum);
   
   recvsize = getSn_RxMAX(chipNum,sn);
   if(recvsize < len) len = recvsize;
   while(1)
   {
      recvsize = getSn_RX_RSR(chipNum,sn);
      tmp = getSn_SR(chipNum,sn);
      if (tmp != SOCK_ESTABLISHED)
      {
         if(tmp == SOCK_CLOSE_WAIT)
         {
            if(recvsize != 0) break;
            else if(getSn_TX_FSR(chipNum,sn) == getSn_TxMAX(chipNum,sn))
            {
               close(chipNum,sn);
               return SOCKERR_SOCKSTATUS;
            }
         }
         else
         {
            close(chipNum,sn);
            return SOCKERR_SOCKSTATUS;
         }
      }
      if((sock_io_mode[chipNum] & (1<<sn)) && (recvsize == 0)) return SOCK_BUSY;
      if(recvsize != 0) break;
   };
   if(recvsize < len) len = recvsize;
   wiz_recv_data(chipNum,sn, buf, len);
   setSn_CR(chipNum,sn,Sn_CR_RECV);
   while(getSn_CR(chipNum,sn));
   return len;
}

int32_t sendto(uint8_t chipNum, uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t port)
{
   uint8_t tmp = 0;
   uint16_t freesize = 0;
   CHECK_SOCKNUM(chipNum,sn);
   switch(getSn_MR(chipNum,sn) & 0x0F)
   {
      case Sn_MR_UDP:
      case Sn_MR_MACRAW:
         break;
      default:
         return SOCKERR_SOCKMODE;
   }
   CHECK_SOCKDATA(chipNum);
   //M20140501 : For avoiding fatal error on memory align mismatched
   //if(*((uint32_t*)addr) == 0) return SOCKERR_IPINVALID;
   {
      uint32_t taddr;
      taddr = ((uint32_t)addr[0]) & 0x000000FF;
      taddr = (taddr << 8) + ((uint32_t)addr[1] & 0x000000FF);
      taddr = (taddr << 8) + ((uint32_t)addr[2] & 0x000000FF);
      taddr = (taddr << 8) + ((uint32_t)addr[3] & 0x000000FF);
   }
   //
   if(*((uint32_t*)addr) == 0) return SOCKERR_IPINVALID;
   if(port == 0)               return SOCKERR_PORTZERO;
   tmp = getSn_SR(chipNum,sn);
   if(tmp != SOCK_MACRAW && tmp != SOCK_UDP) return SOCKERR_SOCKSTATUS;
      
   setSn_DIPR(chipNum,sn,addr);
   setSn_DPORT(chipNum,sn,port);      
   freesize = getSn_TxMAX(chipNum,sn);
   if (len > freesize) len = freesize; // check size not to exceed MAX size.
   while(1)
   {
      freesize = getSn_TX_FSR(chipNum,sn);
      if(getSn_SR(chipNum,sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
      if( (sock_io_mode[chipNum] & (1<<sn)) && (len > freesize) ) return SOCK_BUSY;
      if(len <= freesize) break;
   };
	wiz_send_data(chipNum,sn, buf, len);
   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR(chipNum,0);
   #endif

	setSn_CR(chipNum,sn,Sn_CR_SEND);
	/* wait to process the command... */
	while(getSn_CR(chipNum,sn)){};
   #if _WIZCHIP_ == 5200   // for W5200 ARP errata 
      setSUBR(chipNum,(uint8_t*)"\x00\x00\x00\x00");
   #endif
   while(1)
   {
      tmp = getSn_IR(chipNum,sn);
      if(tmp & Sn_IR_SENDOK)
      {
         setSn_IR(chipNum,sn, Sn_IR_SENDOK);
         break;
      }
      //M:20131104
      //else if(tmp & Sn_IR_TIMEOUT) return SOCKERR_TIMEOUT;
      else if(tmp & Sn_IR_TIMEOUT)
      {
         setSn_IR(chipNum,sn, Sn_IR_TIMEOUT);
         return SOCKERR_TIMEOUT;
      }
      ////////////
   }
	return len;
}



int32_t recvfrom(uint8_t chipNum, uint8_t sn, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t *port)
{
   uint8_t  mr;
   uint8_t  head[8];
	uint16_t pack_len=0;

   CHECK_SOCKNUM(chipNum,sn);
   //CHECK_SOCKMODE(Sn_MR_UDP);
   switch((mr=getSn_MR(chipNum,sn)) & 0x0F)
   {
      case Sn_MR_UDP:
      case Sn_MR_MACRAW:
         break;
   #if ( _WIZCHIP_ < 5200 )         
      case Sn_MR_IPRAW:
      case Sn_MR_PPPoE:
         break;
   #endif
      default:
         return SOCKERR_SOCKMODE;
   }
   CHECK_SOCKDATA(chipNum);
   if(sock_remained_size[chipNum][sn] == 0)
   {
      while(1)
      {
         pack_len = getSn_RX_RSR(chipNum,sn);
         if(getSn_SR(chipNum,sn) == SOCK_CLOSED) return SOCKERR_SOCKCLOSED;
         if( (sock_io_mode[chipNum] & (1<<sn)) && (pack_len == 0) ) return SOCK_BUSY;
         if(pack_len != 0) break;
      };
   }
   sock_pack_info[chipNum][sn] = PACK_COMPLETED;
	switch (mr & 0x07)
	{
	   case Sn_MR_UDP :
	      if(sock_remained_size[chipNum][sn] == 0)
	      {
   			wiz_recv_data(chipNum,sn, head, 8);
   			setSn_CR(chipNum,sn,Sn_CR_RECV);
   			while(getSn_CR(chipNum,sn));
   			// read peer's IP address, port number & packet length
    		addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			*port = head[4];
   			*port = (*port << 8) + head[5];
   			sock_remained_size[chipNum][sn] = head[6];
   			sock_remained_size[chipNum][sn] = (sock_remained_size[chipNum][sn] << 8) + head[7];
   			sock_pack_info[chipNum][sn] = PACK_FIRST;
   	   }
			if(len < sock_remained_size[chipNum][sn]) pack_len = len;
			else pack_len = sock_remained_size[chipNum][sn];
			//
			// Need to packet length check (default 1472)
			//
   		wiz_recv_data(chipNum,sn, buf, pack_len); // data copy.
			break;
	   case Sn_MR_MACRAW :
	      if(sock_remained_size[chipNum][sn] == 0)
	      {
   			wiz_recv_data(chipNum ,sn, head, 2);
   			setSn_CR(chipNum,sn,Sn_CR_RECV);
   			while(getSn_CR(chipNum,sn)){};
   			// read peer's IP address, port number & packet length
    			sock_remained_size[chipNum][sn] = head[0];
   			sock_remained_size[chipNum][sn] = (sock_remained_size[chipNum][sn] <<8) + head[1];
   			if(sock_remained_size[chipNum][sn] > 1514) 
   			{
   			   close(chipNum,sn);
   			   return SOCKFATAL_PACKLEN;
   			}
   			sock_pack_info[chipNum][sn] = PACK_FIRST;
   	   }
			if(len < sock_remained_size[chipNum][sn]) pack_len = len;
			else pack_len = sock_remained_size[chipNum][sn];
			wiz_recv_data(chipNum,sn,buf,pack_len);
		   break;
   #if ( _WIZCHIP_ < 5200 )
		case Sn_MR_IPRAW:
		   if(sock_remained_size[chipNum][sn] == 0)
		   {
   			wiz_recv_data(chipNum,sn, head, 6);
   			setSn_CR(chipNum,sn,Sn_CR_RECV);
   			while(getSn_CR(chipNum,sn));
   			addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			sock_remained_size[chipNum][sn] = head[4];
   			sock_remaiend_size[chipNum][sn] = (sock_remained_size[chipNum][sn] << 8) + head[5];
   			sock_pack_info[chipNum][sn] = PACK_FIRST;
         }
			//
			// Need to packet length check
			//
			if(len < sock_remained_size[chipNum][sn]) pack_len = len;
			else pack_len = sock_remained_size[chipNum][sn];
   		wiz_recv_data(chipNum,sn, buf, pack_len); // data copy.
			break;
   #endif
      default:
         wiz_recv_ignore(chipNum,sn, pack_len); // data copy.
         sock_remained_size[chipNum][sn] = pack_len;
         break;
   }
	setSn_CR(chipNum,sn,Sn_CR_RECV);
	/* wait to process the command... */
	while(getSn_CR(chipNum,sn)) ;
	sock_remained_size[chipNum][sn] -= pack_len;
	//M20140501 : replace 0x01 with PACK_REMAINED
	//if(sock_remained_size[sn] != 0) sock_pack_info[sn] |= 0x01;
	if(sock_remained_size[chipNum][sn] != 0) sock_pack_info[chipNum][sn] |= PACK_REMAINED;
   //
 	return pack_len;
}


int8_t ctlsocket(uint8_t chipNum, uint8_t sn, ctlsock_type cstype, void* arg)
{
   uint8_t tmp = 0;
   CHECK_SOCKNUM(chipNum,sn);
   switch(cstype)
   {
      case CS_SET_IOMODE:
         tmp = *((uint8_t*)arg);
         if(tmp == SOCK_IO_NONBLOCK)  sock_io_mode[chipNum] |= (1<<sn);
         else if(tmp == SOCK_IO_BLOCK) sock_io_mode[chipNum] &= ~(1<<sn);
         else return SOCKERR_ARG;
         break;
      case CS_GET_IOMODE:   
         //M20140501 : implict type casting -> explict type casting
         //*((uint8_t*)arg) = (sock_io_mode >> sn) & 0x0001;
         *((uint8_t*)arg) = (uint8_t)((sock_io_mode[chipNum] >> sn) & 0x0001);
         //
         break;
      case CS_GET_MAXTXBUF:
         *((uint16_t*)arg) = getSn_TxMAX(chipNum,sn);
         break;
      case CS_GET_MAXRXBUF:    
         *((uint16_t*)arg) = getSn_RxMAX(chipNum,sn);
         break;
      case CS_CLR_INTERRUPT:
         if( (*(uint8_t*)arg) > SIK_ALL) return SOCKERR_ARG;
         setSn_IR(chipNum,sn,*(uint8_t*)arg);
         break;
      case CS_GET_INTERRUPT:
         *((uint8_t*)arg) = getSn_IR(chipNum,sn);
         break;
      case CS_SET_INTMASK:  
         if( (*(uint8_t*)arg) > SIK_ALL) return SOCKERR_ARG;
         setSn_IMR(chipNum,sn,*(uint8_t*)arg);
         break;
      case CS_GET_INTMASK:   
         *((uint8_t*)arg) = getSn_IMR(chipNum,sn);
      default:
         return SOCKERR_ARG;
   }
   return SOCK_OK;
}

int8_t setsockopt(uint8_t chipNum, uint8_t sn, sockopt_type sotype, void* arg)
{
 // M20131220 : Remove warning
 //uint8_t tmp;
   CHECK_SOCKNUM(chipNum,sn);
   switch(sotype)
   {
      case SO_TTL:
         setSn_TTL(chipNum,sn,*(uint8_t*)arg);
         break;
      case SO_TOS:
         setSn_TOS(chipNum,sn,*(uint8_t*)arg);
         break;
      case SO_MSS:
         setSn_MSSR(chipNum,sn,*(uint16_t*)arg);
         break;
      case SO_DESTIP:
         setSn_DIPR(chipNum,sn, (uint8_t*)arg);
         break;
      case SO_DESTPORT:
         setSn_DPORT(chipNum,sn, *(uint16_t*)arg);
         break;
#if _WIZCHIP_ != 5100
      case SO_KEEPALIVESEND:
         CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
         #if _WIZCHIP_ > 5200
            if(getSn_KPALVTR(chipNum,sn) != 0) return SOCKERR_SOCKOPT;
         #endif
            setSn_CR(chipNum,sn,Sn_CR_SEND_KEEP);
            while(getSn_CR(chipNum,sn) != 0)
            {
               // M20131220
         		//if ((tmp = getSn_IR(sn)) & Sn_IR_TIMEOUT)
               if (getSn_IR(chipNum,sn) & Sn_IR_TIMEOUT)
         		{
         			setSn_IR(chipNum,sn, Sn_IR_TIMEOUT);
                  return SOCKERR_TIMEOUT;
         		}
            }
         break;
   #if _WIZCHIP_ > 5200
      case SO_KEEPALIVEAUTO:
         CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
         setSn_KPALVTR(chipNum,sn,*(uint8_t*)arg);
         break;
   #endif      
#endif   
      default:
         return SOCKERR_ARG;
   }   
   return SOCK_OK;
}

int8_t getsockopt(uint8_t chipNum, uint8_t sn, sockopt_type sotype, void* arg)
{
   CHECK_SOCKNUM(chipNum,sn);
   switch(sotype)
   {
      case SO_FLAG:
         *(uint8_t*)arg = getSn_MR(chipNum,sn) & 0xF0;
         break;
      case SO_TTL:
         *(uint8_t*) arg = getSn_TTL(chipNum,sn);
         break;
      case SO_TOS:
         *(uint8_t*) arg = getSn_TOS(chipNum,sn);
         break;
      case SO_MSS:   
         *(uint8_t*) arg = getSn_MSSR(chipNum,sn);
      case SO_DESTIP:
         getSn_DIPR(chipNum,sn, (uint8_t*)arg);
         break;
      case SO_DESTPORT:  
         *(uint16_t*) arg = getSn_DPORT(chipNum,sn);
         break;
   #if _WIZCHIP_ > 5200   
      case SO_KEEPALIVEAUTO:
         CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
         *(uint16_t*) arg = getSn_KPALVTR(chipNum,sn);
         break;
   #endif      
      case SO_SENDBUF:
         *(uint16_t*) arg = getSn_TX_FSR(chipNum,sn);
      case SO_RECVBUF:
         *(uint16_t*) arg = getSn_RX_RSR(chipNum,sn);
      case SO_STATUS:
         *(uint8_t*) arg = getSn_SR(chipNum,sn);
         break;
      case SO_REMAINSIZE:
         if(getSn_MR(chipNum,sn) == Sn_MR_TCP)
            *(uint16_t*)arg = getSn_RX_RSR(chipNum,sn);
         else
            *(uint16_t*)arg = sock_remained_size[chipNum][sn];
         break;
      case SO_PACKINFO:
         CHECK_SOCKMODE(chipNum,Sn_MR_TCP);
         *(uint8_t*)arg = sock_pack_info[chipNum][sn];
         break;
      default:
         return SOCKERR_SOCKOPT;
   }
   return SOCK_OK;
}
