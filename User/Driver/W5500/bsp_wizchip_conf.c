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
#include <stddef.h>
#include "bsp_wizchip_conf.h"

/**
 * @brief Default function to enable interrupt.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	  wizchip_cris_enter(void)           {};
/**
 * @brief Default function to disable interrupt.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	  wizchip_cris_exit(void)          {};
/**
 * @brief Default function to select chip.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_cs_select(void)            {};
/**
 * @brief Default function to deselect chip.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_cs_deselect(void)          {};
/**
 * @brief Default function to read in direct or indirect interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
 //M20140501 : Explict pointer type casting
//uint8_t wizchip_bus_readbyte(uint32_t AddrSel) { return * ((volatile uint8_t *) AddrSel); };
uint8_t wizchip_bus_readbyte(uint32_t AddrSel) { return * ((volatile uint8_t *)((ptrdiff_t) AddrSel)); };
/**
 * @brief Default function to write in direct or indirect interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
 
//M20140501 : Explict pointer type casting
//void 	wizchip_bus_writebyte(uint32_t AddrSel, uint8_t wb)  { *((volatile uint8_t*) AddrSel) = wb; };
void 	wizchip_bus_writebyte(uint32_t AddrSel, uint8_t wb)  { *((volatile uint8_t*)((ptrdiff_t)AddrSel)) = wb; };

/**
 * @brief Default function to read in SPI interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
uint8_t wizchip_spi_readbyte(void)        {return 0;};
/**
 * @brief Default function to write in SPI interface.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void 	wizchip_spi_writebyte(uint8_t wb) {};

/**
 * @\ref _WIZCHIP instance
 */
_WIZCHIP  WIZCHIP[2] =
      {
         {
         .id                  = _WIZCHIP_ID_,
         .if_mode             = _WIZCHIP_IO_MODE_,
         .CRIS._enter         = wizchip_cris_enter,
         .CRIS._exit          = wizchip_cris_exit,
         .CS._select          = wizchip_cs_select,
         .CS._deselect        = wizchip_cs_deselect,
//         .IF.BUS._read_byte   = wizchip_bus_readbyte,
//         .IF.BUS._write_byte  = wizchip_bus_writebyte,
         .IF.SPI._read_byte   = wizchip_spi_readbyte,
         .IF.SPI._write_byte  = wizchip_spi_writebyte
         },
         {
         .id                  = _WIZCHIP_ID_,
         .if_mode             = _WIZCHIP_IO_MODE_,
         .CRIS._enter         = wizchip_cris_enter,
         .CRIS._exit          = wizchip_cris_exit,
         .CS._select          = wizchip_cs_select,
         .CS._deselect        = wizchip_cs_deselect,
//         .IF.BUS._read_byte   = wizchip_bus_readbyte,
//         .IF.BUS._write_byte  = wizchip_bus_writebyte,
         .IF.SPI._read_byte   = wizchip_spi_readbyte,
         .IF.SPI._write_byte  = wizchip_spi_writebyte
         }
      };


static uint8_t    _DNS_chip1[4];      // DNS server ip address
static uint8_t    _DNS_chip2[4];      // DNS server ip address
static dhcp_mode  _DHCP_chip1;        // DHCP mode
static dhcp_mode  _DHCP_chip2;        // DHCP mode

void reg_wizchip_cris_cbfunc(uint8_t chipNum, void(*cris_en)(void), void(*cris_ex)(void))
{
   if((chipNum == 0) || (chipNum == 1))
   {
      if(!cris_en || !cris_ex)
      {
         WIZCHIP[chipNum].CRIS._enter = wizchip_cris_enter;
         WIZCHIP[chipNum].CRIS._exit  = wizchip_cris_exit;
      }
      else
      {
         WIZCHIP[chipNum].CRIS._enter = cris_en;
         WIZCHIP[chipNum].CRIS._exit  = cris_ex;
      }
   }
}

void reg_wizchip_cs_cbfunc(uint8_t chipNum, void(*cs_sel)(void), void(*cs_desel)(void))
{
   if((chipNum == 0) || (chipNum == 1))
   {
      if(!cs_sel || !cs_desel)
      {
         WIZCHIP[chipNum].CS._select   = wizchip_cs_select;
         WIZCHIP[chipNum].CS._deselect = wizchip_cs_deselect;
      }
      else
      {
         WIZCHIP[chipNum].CS._select   = cs_sel;
         WIZCHIP[chipNum].CS._deselect = cs_desel;
      }
   }
}

void reg_wizchip_bus_cbfunc(uint8_t chipNum, uint8_t(*bus_rb)(uint32_t addr), void (*bus_wb)(uint32_t addr, uint8_t wb))
{
   if((chipNum == 0) || (chipNum == 1))
   {
      while(!(WIZCHIP[chipNum].if_mode & _WIZCHIP_IO_MODE_BUS_));
      
      if(!bus_rb || !bus_wb)
      {
         WIZCHIP[chipNum].IF.BUS._read_byte   = wizchip_bus_readbyte;
         WIZCHIP[chipNum].IF.BUS._write_byte  = wizchip_bus_writebyte;
      }
      else
      {
         WIZCHIP[chipNum].IF.BUS._read_byte   = bus_rb;
         WIZCHIP[chipNum].IF.BUS._write_byte  = bus_wb;
      }
   }
}

void reg_wizchip_spi_cbfunc(uint8_t chipNum, uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb))
{
   if((chipNum == 0) || (chipNum == 1))
   {
      while(!(WIZCHIP[chipNum].if_mode & _WIZCHIP_IO_MODE_SPI_));
      
      if(!spi_rb || !spi_wb)
      {
         WIZCHIP[chipNum].IF.SPI._read_byte   = wizchip_spi_readbyte;
         WIZCHIP[chipNum].IF.SPI._write_byte  = wizchip_spi_writebyte;
      }
      else
      {
         WIZCHIP[chipNum].IF.SPI._read_byte   = spi_rb;
         WIZCHIP[chipNum].IF.SPI._write_byte  = spi_wb;
      }
   }
}

int8_t ctlwizchip(uint8_t chipNum,ctlwizchip_type cwtype, void* arg)
{
   uint8_t tmp = 0;
   uint8_t* ptmp[2] = {0,0};

   if((chipNum == 0) || (chipNum == 1))
   {
      switch(cwtype)
      {
         case CW_RESET_WIZCHIP:
            wizchip_sw_reset(chipNum);
            break;
         case CW_INIT_WIZCHIP:
            if(arg != 0) 
            {
               ptmp[0] = (uint8_t*)arg;
               ptmp[1] = ptmp[0] + _WIZCHIP_SOCK_NUM_;
            }
            return wizchip_init(chipNum, ptmp[0], ptmp[1]);
         case CW_CLR_INTERRUPT:
            wizchip_clrinterrupt(chipNum, *((intr_kind*)arg));
            break;
         case CW_GET_INTERRUPT:
         *((intr_kind*)arg) = wizchip_getinterrupt(chipNum);
            break;
         case CW_SET_INTRMASK:
            wizchip_setinterruptmask(chipNum, *((intr_kind*)arg));
            break;         
         case CW_GET_INTRMASK:
            *((intr_kind*)arg) = wizchip_getinterruptmask(chipNum);
            break;
      #if _WIZCHIP_ > 5100
         case CW_SET_INTRTIME:
            setINTLEVEL(chipNum,*(uint16_t*)arg);
            break;
         case CW_GET_INTRTIME:
            *(uint16_t*)arg = getINTLEVEL(chipNum);
            break;
      #endif
         case CW_GET_ID:
               ((uint8_t*)arg)[0] = WIZCHIP[chipNum].id[0];
               ((uint8_t*)arg)[1] = WIZCHIP[chipNum].id[1];
               ((uint8_t*)arg)[2] = WIZCHIP[chipNum].id[2];
               ((uint8_t*)arg)[3] = WIZCHIP[chipNum].id[3];
               ((uint8_t*)arg)[4] = WIZCHIP[chipNum].id[4];
               ((uint8_t*)arg)[5] = 0;
            break;
      #if _WIZCHIP_ ==  5500
         case CW_RESET_PHY:
            wizphy_reset(chipNum);
            break;
         case CW_SET_PHYCONF:
            wizphy_setphyconf(chipNum,(wiz_PhyConf*)arg);
            break;
         case CW_GET_PHYCONF:
            wizphy_getphyconf(chipNum,(wiz_PhyConf*)arg);
            break;
         case CW_GET_PHYSTATUS:
            break;
         case CW_SET_PHYPOWMODE:
            return wizphy_setphypmode(chipNum, *(uint8_t*)arg);
      #endif
         case CW_GET_PHYPOWMODE:
            tmp = wizphy_getphypmode(chipNum);
            if((int8_t)tmp == -1) return -1;
            *(uint8_t*)arg = tmp;
            break;
         case CW_GET_PHYLINK:
            tmp = wizphy_getphylink(chipNum);
            if((int8_t)tmp == -1) return -1;
            *(uint8_t*)arg = tmp;
            break;
         default:
            return -1;
      }
      return 0;
   }
   else return -1;
}


int8_t ctlnetwork(uint8_t chipNum, ctlnetwork_type cntype, void* arg)
{
   if((chipNum == 0) || (chipNum == 1))
   {
      switch(cntype)
      {
         case CN_SET_NETINFO:
            wizchip_setnetinfo(chipNum,(wiz_NetInfo*)arg);
            break;
         case CN_GET_NETINFO:
            wizchip_getnetinfo(chipNum,(wiz_NetInfo*)arg);
            break;
         case CN_SET_NETMODE:
            return wizchip_setnetmode(chipNum,*(netmode_type*)arg);
         case CN_GET_NETMODE:
            *(netmode_type*)arg = wizchip_getnetmode(chipNum);
            break;
         case CN_SET_TIMEOUT:
            wizchip_settimeout(chipNum,(wiz_NetTimeout*)arg);
            break;
         case CN_GET_TIMEOUT:
            wizchip_gettimeout(chipNum,(wiz_NetTimeout*)arg);
            break;
         default:
            return -1;
      }
      return 0;
   }
   else return -1;
}

void wizchip_sw_reset(uint8_t chipNum)
{
   uint8_t gw[4], sn[4], sip[4];
   uint8_t mac[6];
   getSHAR(chipNum,mac);
   getGAR(chipNum,gw);  getSUBR(chipNum,sn);  getSIPR(chipNum,sip);
   setMR(chipNum,MR_RST);
   getMR(chipNum); // for delay
   setSHAR(chipNum,mac);
   setGAR(chipNum,gw);
   setSUBR(chipNum,sn);
   setSIPR(chipNum,sip);
}

int8_t wizchip_init(uint8_t chipNum, uint8_t* txsize, uint8_t* rxsize)
{
   int8_t i;
   int8_t tmp = 0;
   wizchip_sw_reset(chipNum);
   if(txsize)
   {
      tmp = 0;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         tmp += txsize[i];
      if(tmp > 16) return -1;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         setSn_TXBUF_SIZE(chipNum, i, txsize[i]);
   }
   if(rxsize)
   {
      tmp = 0;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         tmp += rxsize[i];
      if(tmp > 16) return -1;
      for(i = 0 ; i < _WIZCHIP_SOCK_NUM_; i++)
         setSn_RXBUF_SIZE(chipNum, i, rxsize[i]);
   }
   return 0;
}

void wizchip_clrinterrupt(uint8_t chipNum, intr_kind intr)
{
   uint8_t ir  = (uint8_t)intr;
   uint8_t sir = (uint8_t)((uint16_t)intr >> 8);
#if _WIZCHIP_ < 5500
   ir |= (1<<4); // IK_WOL
#endif
#if _WIZCHIP_ == 5200
   ir |= (1 << 6);
#endif
   
#if _WIZCHIP_ < 5200
   sir &= 0x0F;
#endif

#if _WIZCHIP_ == 5100
   ir |= sir;
   setIR(chipNum,ir);
#else
   setIR(chipNum,ir);
   setSIR(chipNum,sir);
#endif   
}

intr_kind wizchip_getinterrupt(uint8_t chipNum)
{
   uint8_t ir  = 0;
   uint8_t sir = 0;
   uint16_t ret = 0;
#if _WIZCHIP_ == 5100
   ir = getIR(chipNum);
   sir = ir 0x0F;
#else
   ir  = getIR(chipNum);
   sir = getSIR(chipNum);
#endif         

#if _WIZCHIP_ < 5500
   ir &= ~(1<<4); // IK_WOL
#endif
#if _WIZCHIP_ == 5200
   ir &= ~(1 << 6);
#endif
  ret = sir;
  ret = (ret << 8) + ir;
  return (intr_kind)ret;
}

void wizchip_setinterruptmask(uint8_t chipNum, intr_kind intr)
{
   uint8_t imr  = (uint8_t)intr;
   uint8_t simr = (uint8_t)((uint16_t)intr >> 8);
#if _WIZCHIP_ < 5500
   imr &= ~(1<<4); // IK_WOL
#endif
#if _WIZCHIP_ == 5200
   imr &= ~(1 << 6);
#endif
   
#if _WIZCHIP_ < 5200
   simr &= 0x0F;
#endif

#if _WIZCHIP_ == 5100
   imr |= simr;
   setIMR(chipNum,imr);
#else
   setIMR(chipNum,imr);
   setSIMR(chipNum,simr);
#endif   
}

intr_kind wizchip_getinterruptmask(uint8_t chipNum)
{
   uint8_t imr  = 0;
   uint8_t simr = 0;
   uint16_t ret = 0;
#if _WIZCHIP_ == 5100
   imr  = getIMR(chipNum);
   simr = imr 0x0F;
#else
   imr  = getIMR(chipNum);
   simr = getSIMR(chipNum);
#endif         

#if _WIZCHIP_ < 5500
   imr &= ~(1<<4); // IK_WOL
#endif
#if _WIZCHIP_ == 5200
   imr &= ~(1 << 6);  // IK_DEST_UNREACH
#endif
  ret = simr;
  ret = (ret << 8) + imr;
  return (intr_kind)ret;
}

int8_t wizphy_getphylink(uint8_t chipNum)
{
   int8_t tmp;
#if   _WIZCHIP_ == 5200
   if(getPHYSTATUS(chipNum) & PHYSTATUS_LINK)
      tmp = PHY_LINK_ON;
   else
      tmp = PHY_LINK_OFF;
#elif _WIZCHIP_ == 5500
   if(getPHYCFGR(chipNum) & PHYCFGR_LNK_ON)
      tmp = PHY_LINK_ON;
   else
      tmp = PHY_LINK_OFF;
#else
   tmp = -1;
#endif
   return tmp;
}

#if _WIZCHIP_ > 5100

int8_t wizphy_getphypmode(uint8_t chipNum)
{
   int8_t tmp = 0;
   #if   _WIZCHIP_ == 5200
      if(getPHYSTATUS(chipNum) & PHYSTATUS_POWERDOWN)
         tmp = PHY_POWER_DOWN;
      else          
         tmp = PHY_POWER_NORM;
   #elif _WIZCHIP_ == 5500
      if(getPHYCFGR(chipNum) & PHYCFGR_OPMDC_PDOWN)
         tmp = PHY_POWER_DOWN;
      else 
         tmp = PHY_POWER_NORM;
   #else
      tmp = -1;
   #endif
   return tmp;
}
#endif

#if _WIZCHIP_ == 5500
void wizphy_reset(uint8_t chipNum)
{
   uint8_t tmp = getPHYCFGR(chipNum);
   tmp &= PHYCFGR_RST;
   setPHYCFGR(chipNum,tmp);
   tmp = getPHYCFGR(chipNum); 
   tmp |= ~PHYCFGR_RST;
   setPHYCFGR(chipNum,tmp);
}

void wizphy_setphyconf(uint8_t chipNum, wiz_PhyConf* phyconf)
{
   uint8_t tmp = 0;
   if(phyconf->by == PHY_CONFBY_SW)
      tmp |= PHYCFGR_OPMD;
   else
      tmp &= ~PHYCFGR_OPMD;
   if(phyconf->mode == PHY_MODE_AUTONEGO)
      tmp |= PHYCFGR_OPMDC_ALLA;
   else
   {
      if(phyconf->duplex == PHY_DUPLEX_FULL)
      {
         if(phyconf->speed == PHY_SPEED_100)
            tmp |= PHYCFGR_OPMDC_100F;
         else
            tmp |= PHYCFGR_OPMDC_10F;
      }   
      else
      {
         if(phyconf->speed == PHY_SPEED_100)
            tmp |= PHYCFGR_OPMDC_100H;
         else
            tmp |= PHYCFGR_OPMDC_10H;
      }
   }
   setPHYCFGR(chipNum,tmp);
   wizphy_reset(chipNum);
}

void wizphy_getphyconf(uint8_t chipNum, wiz_PhyConf* phyconf)
{
   uint8_t tmp = 0;
   tmp = getPHYCFGR(chipNum);
   phyconf->by   = (tmp & PHYCFGR_OPMD) ? PHY_CONFBY_SW : PHY_CONFBY_HW;
   switch(tmp & PHYCFGR_OPMDC_ALLA)
   {
      case PHYCFGR_OPMDC_ALLA:
      case PHYCFGR_OPMDC_100FA: 
         phyconf->mode = PHY_MODE_AUTONEGO;
         break;
      default:
         phyconf->mode = PHY_MODE_MANUAL;
         break;
   }
   switch(tmp & PHYCFGR_OPMDC_ALLA)
   {
      case PHYCFGR_OPMDC_100FA:
      case PHYCFGR_OPMDC_100F:
      case PHYCFGR_OPMDC_100H:
         phyconf->speed = PHY_SPEED_100;
         break;
      default:
         phyconf->speed = PHY_SPEED_10;
         break;
   }
   switch(tmp & PHYCFGR_OPMDC_ALLA)
   {
      case PHYCFGR_OPMDC_100FA:
      case PHYCFGR_OPMDC_100F:
      case PHYCFGR_OPMDC_10F:
         phyconf->duplex = PHY_DUPLEX_FULL;
         break;
      default:
         phyconf->duplex = PHY_DUPLEX_HALF;
         break;
   }
}

void wizphy_getphystat(uint8_t chipNum, wiz_PhyConf* phyconf)
{
   uint8_t tmp = getPHYCFGR(chipNum);
   phyconf->duplex = (tmp & PHYCFGR_DPX_FULL) ? PHY_DUPLEX_FULL : PHY_DUPLEX_HALF;
   phyconf->speed  = (tmp & PHYCFGR_SPD_100) ? PHY_SPEED_100 : PHY_SPEED_10;
}

int8_t wizphy_setphypmode(uint8_t chipNum, uint8_t pmode)
{
   uint8_t tmp = 0;
   tmp = getPHYCFGR(chipNum);
   if((tmp & PHYCFGR_OPMD)== 0) return -1;
   tmp &= ~PHYCFGR_OPMDC_ALLA;         
   if( pmode == PHY_POWER_DOWN)
      tmp |= PHYCFGR_OPMDC_PDOWN;
   else
      tmp |= PHYCFGR_OPMDC_ALLA;
   setPHYCFGR(chipNum,tmp);
   wizphy_reset(chipNum);
   tmp = getPHYCFGR(chipNum);
   if( pmode == PHY_POWER_DOWN)
   {
      if(tmp & PHYCFGR_OPMDC_PDOWN) return 0;
   }
   else
   {
      if(tmp & PHYCFGR_OPMDC_ALLA) return 0;
   }
   return -1;
}
#endif


void wizchip_setnetinfo(uint8_t chipNum, wiz_NetInfo* pnetinfo)
{
   setSHAR(chipNum,pnetinfo->mac);
   setGAR(chipNum,pnetinfo->gw);
   setSUBR(chipNum,pnetinfo->sn);
   setSIPR(chipNum,pnetinfo->ip);
   if(chipNum == 0)
   {
      _DNS_chip1[0] = pnetinfo->dns[0];
      _DNS_chip1[1] = pnetinfo->dns[1];
      _DNS_chip1[2] = pnetinfo->dns[2];
      _DNS_chip1[3] = pnetinfo->dns[3];
      _DHCP_chip1   = pnetinfo->dhcp;
   }
   else
   {
      _DNS_chip2[0] = pnetinfo->dns[0];
      _DNS_chip2[1] = pnetinfo->dns[1];
      _DNS_chip2[2] = pnetinfo->dns[2];
      _DNS_chip2[3] = pnetinfo->dns[3];
      _DHCP_chip2   = pnetinfo->dhcp;
   }
}

void wizchip_getnetinfo(uint8_t chipNum, wiz_NetInfo* pnetinfo)
{
   getSHAR(chipNum,pnetinfo->mac);
   getGAR(chipNum,pnetinfo->gw);
   getSUBR(chipNum,pnetinfo->sn);
   getSIPR(chipNum,pnetinfo->ip);
   if(chipNum == 0)
   {
      pnetinfo->dns[0]= _DNS_chip1[0];
      pnetinfo->dns[1]= _DNS_chip1[1];
      pnetinfo->dns[2]= _DNS_chip1[2];
      pnetinfo->dns[3]= _DNS_chip1[3];
      pnetinfo->dhcp  = _DHCP_chip1;
   }
   else
   {
      pnetinfo->dns[0]= _DNS_chip2[0];
      pnetinfo->dns[1]= _DNS_chip2[1];
      pnetinfo->dns[2]= _DNS_chip2[2];
      pnetinfo->dns[3]= _DNS_chip2[3];
      pnetinfo->dhcp  = _DHCP_chip2;
   }
}

int8_t wizchip_setnetmode(uint8_t chipNum, netmode_type netmode)
{
   uint8_t tmp = 0;
#if _WIZCHIP_ != 5500   
   if(netmode & ~(NM_WAKEONLAN | NM_PPPOE | NM_PINGBLOCK)) return -1;
#else
   if(netmode & ~(NM_WAKEONLAN | NM_PPPOE | NM_PINGBLOCK | NM_FORCEARP)) return -1;
#endif      
   tmp = getMR(chipNum);
   tmp |= (uint8_t)netmode;
   setMR(chipNum,tmp);
   return 0;
}

netmode_type wizchip_getnetmode(uint8_t chipNum)
{
   return (netmode_type) getMR(chipNum);
}

void wizchip_settimeout(uint8_t chipNum, wiz_NetTimeout* nettime)
{
   setRCR(chipNum, nettime->retry_cnt);
   setRTR(chipNum, nettime->time_100us);
}

void wizchip_gettimeout(uint8_t chipNum, wiz_NetTimeout* nettime)
{
   nettime->retry_cnt = getRCR(chipNum);
   nettime->time_100us = getRTR(chipNum);
}
