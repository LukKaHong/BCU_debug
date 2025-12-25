/*
*********************************************************************************************************
*
*	ģ������ : ��̫���ӿ���������
*	�ļ����� : bsp_ethernet.c
*	��    �� : V1.0
*	˵    �� : ��̫���ӿ���������,˫W5500
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.1    2021-11-22  wr     ��ʽ����
*
*
*********************************************************************************************************
*/

#include "bsp.h"
//#include "delay.h"
//#include "stm32_spi.h"
#include "stm32h7xx_hal_rng.h"
#include "stm32h7xx_hal_rng_ex.h"
#include "wiz_interface.h"
#include "main.h"
#include "spi.h"

static void ETH1_SPI_WriteByte(uint8_t TxData);
static uint8_t ETH1_SPI_ReadByte(void);
static void ETH1_SPI_CS_Select(void);
static void ETH1_SPI_CS_Deselect(void);
static void ETH1_RESET(void);
static int8_t ETH1_Init(void);
static void ETH1_netSet(void);
static void ETH2_SPI_WriteByte(uint8_t TxData);
static uint8_t ETH2_SPI_ReadByte(void);
static void ETH2_SPI_CS_Select(void);
static void ETH2_SPI_CS_Deselect(void);
static void ETH2_RESET(void);
static int8_t ETH2_Init(void);
static void ETH2_netSet(void);
static void defaultHandle(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen);

#ifndef W5500_TCP_LOCAL_PORT_RANGE_START
/* From http://www.iana.org/assignments/port-numbers:
   "The Dynamic and/or Private Ports are those from 49152 through 65535" */
#define W5500_TCP_LOCAL_PORT_RANGE_START        0xc000
#define W5500_TCP_LOCAL_PORT_RANGE_END          0xffff
#define W5500_TCP_ENSURE_LOCAL_PORT_RANGE(port) ((uint16_t)(((port) & (uint16_t)~W5500_TCP_LOCAL_PORT_RANGE_START) + W5500_TCP_LOCAL_PORT_RANGE_START))
#endif
static RNG_HandleTypeDef rng_handle;
static uint8_t rng_init(void)
{
    uint16_t retry = 0;
 
    rng_handle.Instance = RNG;   
    HAL_RNG_DeInit(&rng_handle);
    HAL_RNG_Init(&rng_handle);    
    
    while ((__HAL_RNG_GET_FLAG(&rng_handle, RNG_FLAG_DRDY) == RESET) && (retry < 10000))
    {
        retry++;
        //vTaskDelay(10);
    }
    if (retry >= 10000)
    {
        return 1; 
    }
    return 0;
}
static uint32_t w5500_rng_get_random_num(void)
{
    uint32_t randomnum;
    HAL_RNG_GenerateRandomNumber(&rng_handle, &randomnum);
    return randomnum;
}
/* last local TCP port */
static uint16_t tcp_port = W5500_TCP_LOCAL_PORT_RANGE_START;

/*����eth������Ϣ��������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
_ethConfig ethConfig[Eth_Num] = {
									{
										.ethID = 0,
					
										.SPI.ID = SPI6,
										.SPI.Baud = SPI_BAUD_12M5,
										.SPI._Init = MX_SPI6_Init,
										.SPI._read_byte = ETH1_SPI_ReadByte,
										.SPI._write_byte = ETH1_SPI_WriteByte,
										.SPI.CS._select = ETH1_SPI_CS_Select,
										.SPI.CS._deselect = ETH1_SPI_CS_Deselect,
										
										.netInfo.mac  = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
										.netInfo.ip   = {172,19,130,201},
										.netInfo.sn   = {255,255,255,0},
										.netInfo.gw   = {172,19,130,1},
										.netInfo.dns  = {0,0,0,0},
										.netInfo.dhcp =  NETINFO_STATIC,
										
										.memSize[0] = {4,4,4,4,0,0,0,0},
										.memSize[1] = {4,4,4,4,0,0,0,0},

										._ethReset = ETH1_RESET,
										._ethInit = ETH1_Init,
										._netSet = ETH1_netSet
									},
									{
										.ethID = 1,
					
										.SPI.ID = SPI5,
										.SPI.Baud = SPI_BAUD_12M5,
										.SPI._Init = MX_SPI6_Init,
										.SPI._read_byte = ETH2_SPI_ReadByte,
										.SPI._write_byte = ETH2_SPI_WriteByte,
										.SPI.CS._select = ETH2_SPI_CS_Select,
										.SPI.CS._deselect = ETH2_SPI_CS_Deselect,
										
										.netInfo.mac  = {0x00, 0x08, 0xdc,0x00, 0xab, 0xce},
										.netInfo.ip   = {192,168,4,13},
										.netInfo.sn   = {255,255,255,0},
										.netInfo.gw   = {192,168,4,1},
										.netInfo.dns  = {0,0,0,0},
										.netInfo.dhcp =  NETINFO_STATIC,
										
										.memSize[0] = {2,2,2,2,2,2,2,2},
										.memSize[1] = {2,2,2,2,2,2,2,2},

										._ethReset = ETH2_RESET,
										._ethInit = ETH2_Init,
										._netSet = ETH2_netSet
									}
								};

/* Tcp Server Socket���ýṹ���ʼ�� */
_tcpserverConfig tcpsConfig[Eth_Num] = {
												{
													.enable = 0,
													.socketID = SOCK_TCPS,
													.serverPort = 5000,
													.funcAddr = defaultHandle
												},
												{
													.enable = 0,
													.socketID = SOCK_TCPS,
													.serverPort = 5000,
													.funcAddr = defaultHandle
												}
											};	
/* Tcp Client Socket���ýṹ���ʼ�� */				
_tcpclientConfig tcpcConfig[Eth_Num] = {
												{
													.enable = 1,
													.socketID = 1,
													.remotePort = 8866,
													.remoteIp =  {172,19,130,109},
													.localPort = 8001,
													.funcAddr = defaultHandle
												},
												{
													.enable = 1,
													.socketID = 2,
													.remotePort = 22233,
													.remoteIp =  {172,19,130,88},
													.localPort = 8001,
													.funcAddr = defaultHandle
												}
											};	

											  
											  
void ethernet_print_none(void)  {return ;};
void (*_ethernet_print)  (void) = ethernet_print_none;

/**
*@brief		client����server�ɹ���ĳ�ʼ��Ϣ��ӡ����
*@param		��ʼ��Ϣ��ӡ����ֱָ��
*@return	��
*/
void reg_ethernet_print_func(void (*eth_print)(void))
{
    
   if(!eth_print)
   {
     _ethernet_print = ethernet_print_none;
   }
   else
   {
      _ethernet_print = eth_print;
   }
}

wiz_NetTimeout sWIZNetTimeout = {.time_100us = 0x64, .retry_cnt = 5};           /* ��ʱ0x64*0.1ms ���� 5��; оƬĬ��Ϊ��ʱ200ms������8�� */


/**
 * @brief Default function to handle in tcpserver or tcpclient.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void defaultHandle(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen) {};
	
/**
  * @brief  �����ٽ���
  * @retval None
  */
void ETH_SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}
/**
  * @brief  �˳��ٽ���
  * @retval None
  */
void ETH_SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
  * @brief  ��SPI���߷���1�ֽ�����
  * @retval ����������
  */
void ETH1_SPI_WriteByte(uint8_t TxData)
{			
	bsp_spiw55002TransferByte(TxData);
}

/**
  * @brief  ��SPI���߶�ȡ1�ֽ�����
  * @retval ����������
  */
uint8_t ETH1_SPI_ReadByte(void)
{		
	return(bsp_spiw55002TransferByte(0x00)); 		 	
}

/**
  * @brief  Ƭѡ�ź�����͵�ƽ
  * @retval None
  */
void ETH1_SPI_CS_Select(void)
{
	 HAL_GPIO_WritePin(A_W5500_SCSn_GPIO_Port, A_W5500_SCSn_Pin, GPIO_PIN_RESET);
}
/**
  * @brief  Ƭѡ�ź�����ߵ�ƽ
  * @retval None
  */
void ETH1_SPI_CS_Deselect(void)
{
	 HAL_GPIO_WritePin(A_W5500_SCSn_GPIO_Port, A_W5500_SCSn_Pin, GPIO_PIN_SET);
}

/**
  * @brief  ��λW5500оƬ
  * @retval None
  */
void ETH1_RESET(void)
{
	HAL_GPIO_WritePin(A_W5500_RSTn_GPIO_Port, A_W5500_RSTn_Pin, GPIO_PIN_RESET); /*��λ�ܽ�����͵�ƽ */
//	bsp_DelayMS(5);
	HAL_Delay(5);
	HAL_GPIO_WritePin(A_W5500_RSTn_GPIO_Port, A_W5500_RSTn_Pin, GPIO_PIN_SET); 	/*��λ�ܽ�����ߵ�ƽ */
//	bsp_DelayMS(50);
	HAL_Delay(50);
}

/*
*********************************************************************************************************
*	�� �� ��: ETH1_netSet
*	����˵��: ����1��������(IP��mask��gate��������Ϣ)
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)
uint8_t ethernet_buf[ETHERNET_BUF_MAX_SIZE] = {0};
void ETH1_netSet(void)
{
	uint8_t chipNum = 0;

	/* Network initialization */
	ctlnetwork(chipNum, CN_SET_NETINFO, (void*)&(ethConfig[0].netInfo));
	// network_init(chipNum, ethernet_buf, (void*)&(ethConfig[0].netInfo));
	ctlnetwork(chipNum, CN_GET_NETINFO, (void*)&(ethConfig[0].netInfo));

	/*����w5500�ĳ�ʱʱ�䣬Ĭ��ֵΪ��ʱ200ms������8�Σ�û����Ҫ���Բ�����*/
	//ctlnetwork(CN_SET_TIMEOUT, (void*)&sWIZNetTimeout);

	/* 5*3��keep alive���ݷ��Ͳ�ͨ��������socket */
	setRTR(chipNum,5);									/* ��ʱʱ�� */
	WIZCHIP_WRITE(chipNum, Sn_KPALVTR(SOCK_TCPS),0x03);	/*��keep aliveģʽ*/
	WIZCHIP_WRITE(chipNum, Sn_KPALVTR(SOCK_TCPC),0x03);	/*��keep aliveģʽ*/	
	
	// Display Network Information
	#ifdef __DEF_IINCHIP_DBG__
		printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",ethConfig[0].netInfo.mac[0],ethConfig[0].netInfo.mac[1],ethConfig[0].netInfo.mac[2],\
		ethConfig[0].netInfo.mac[3],ethConfig[0].netInfo.mac[4],ethConfig[0].netInfo.mac[5]);
		printf("SIP: %d.%d.%d.%d\r\n", ethConfig[0].netInfo.ip[0],ethConfig[0].netInfo.ip[1],ethConfig[0].netInfo.ip[2],ethConfig[0].netInfo.ip[3]);
		printf("GAR: %d.%d.%d.%d\r\n", ethConfig[0].netInfo.gw[0],ethConfig[0].netInfo.gw[1],ethConfig[0].netInfo.gw[2],ethConfig[0].netInfo.gw[3]);
		printf("SUB: %d.%d.%d.%d\r\n", ethConfig[0].netInfo.sn[0],ethConfig[0].netInfo.sn[1],ethConfig[0].netInfo.sn[2],ethConfig[0].netInfo.sn[3]);
		printf("DNS: %d.%d.%d.%d\r\n", ethConfig[0].netInfo.dns[0],ethConfig[0].netInfo.dns[1],ethConfig[0].netInfo.dns[2],ethConfig[0].netInfo.dns[3]);
	#endif
}
/*
*********************************************************************************************************
*	�� �� ��: ETH1_Init
*	����˵��: ����1��ʼ������
*	��    �Σ���
*	�� �� ֵ: 0���ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t ETH1_Init(void)
{
//	uint8_t tmp;
//	unsigned short res=0;
	uint8_t chipNum = 0;
		
	//Host dependent peripheral initialized
	// First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
	/* Critical section callback */
	//	reg_wizchip_cris_cbfunc(ETH_SPI_CrisEnter, ETH_SPI_CrisExit);	//ע���ٽ�������
	/* Chip selection call back */
	reg_wizchip_cs_cbfunc(chipNum,ethConfig[0].SPI.CS._select, ethConfig[0].SPI.CS._deselect);  // CS must be tried with LOW
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(chipNum,ethConfig[0].SPI._read_byte, ethConfig[0].SPI._write_byte);	//ע���д����
	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(chipNum,CW_INIT_WIZCHIP,(void*)ethConfig[0].memSize) == -1) return -1;
	
	/* net set */
	ETH1_netSet();
	
	return 0;
}

/**
  * @brief  ��SPI���߷���1�ֽ�����
  * @retval ����������
  */
void ETH2_SPI_WriteByte(uint8_t TxData)
{			
	bsp_spiw55002TransferByte(TxData);
}

/**
  * @brief  ��SPI���߶�ȡ1�ֽ�����
  * @retval ����������
  */
uint8_t ETH2_SPI_ReadByte(void)
{		
	return(bsp_spiw55002TransferByte(0x00)); 		    
}

/**
  * @brief  Ƭѡ�ź�����͵�ƽ
  * @retval None
  */
void ETH2_SPI_CS_Select(void)
{
//	 HAL_GPIO_WritePin(SPI5_SCS_PORT, SPI5_SCS_PIN, GPIO_PIN_RESET);
}
/**
  * @brief  Ƭѡ�ź�����ߵ�ƽ
  * @retval None
  */
void ETH2_SPI_CS_Deselect(void)
{
//	 HAL_GPIO_WritePin(SPI5_SCS_PORT, SPI5_SCS_PIN, GPIO_PIN_SET);
}

/**
  * @brief  ��λW5500оƬ
  * @retval None
  */
void ETH2_RESET(void)
{
//	HAL_GPIO_WritePin(SPI5_RST_PORT, SPI5_RST_PIN, GPIO_PIN_RESET); /*��λ�ܽ�����͵�ƽ */
////	bsp_DelayMS(5);
//	HAL_Delay(5);
//	HAL_GPIO_WritePin(SPI5_RST_PORT, SPI5_RST_PIN, GPIO_PIN_SET); 	/*��λ�ܽ�����ߵ�ƽ */
////	bsp_DelayMS(50);
//	HAL_Delay(50);
}

/*
*********************************************************************************************************
*	�� �� ��: ETH1_netSet
*	����˵��: ����1��������(IP��mask��gate��������Ϣ)
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ETH2_netSet(void)
{
	uint8_t chipNum = 1;

	/* Network initialization */
	ctlnetwork(chipNum, CN_SET_NETINFO, (void*)&(ethConfig[1].netInfo));
	ctlnetwork(chipNum, CN_GET_NETINFO, (void*)&(ethConfig[1].netInfo));

	/*����w5500�ĳ�ʱʱ�䣬Ĭ��ֵΪ��ʱ200ms������8�Σ�û����Ҫ���Բ�����*/
	//ctlnetwork(CN_SET_TIMEOUT, (void*)&sWIZNetTimeout);

	/* 5*3��keep alive���ݷ��Ͳ�ͨ��������socket */
	setRTR(chipNum,5);									/* ��ʱʱ�� */
	IINCHIP_WRITE(chipNum,Sn_KPALVTR(SOCK_TCPS),0x03);	/*��keep aliveģʽ*/
	IINCHIP_WRITE(chipNum,Sn_KPALVTR(SOCK_TCPC),0x03);	/*��keep aliveģʽ*/	
	
	// Display Network Information
	#ifdef __DEF_IINCHIP_DBG__
		printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",ethConfig[1].netInfo.mac[0],ethConfig[1].netInfo.mac[1],ethConfig[1].netInfo.mac[2],\
		ethConfig[1].netInfo.mac[3],ethConfig[1].netInfo.mac[4],ethConfig[1].netInfo.mac[5]);
		printf("SIP: %d.%d.%d.%d\r\n", ethConfig[1].netInfo.ip[0],ethConfig[1].netInfo.ip[1],ethConfig[1].netInfo.ip[2],ethConfig[1].netInfo.ip[3]);
		printf("GAR: %d.%d.%d.%d\r\n", ethConfig[1].netInfo.gw[0],ethConfig[1].netInfo.gw[1],ethConfig[1].netInfo.gw[2],ethConfig[1].netInfo.gw[3]);
		printf("SUB: %d.%d.%d.%d\r\n", ethConfig[1].netInfo.sn[0],ethConfig[1].netInfo.sn[1],ethConfig[1].netInfo.sn[2],ethConfig[1].netInfo.sn[3]);
		printf("DNS: %d.%d.%d.%d\r\n", ethConfig[1].netInfo.dns[0],ethConfig[1].netInfo.dns[1],ethConfig[1].netInfo.dns[2],ethConfig[1].netInfo.dns[3]);
	#endif
}
/*
*********************************************************************************************************
*	�� �� ��: ETH1_Init
*	����˵��: ����1��ʼ������
*	��    �Σ���
*	�� �� ֵ: 0���ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t ETH2_Init(void)
{
//	uint8_t tmp;
//	unsigned short res=0;
	uint8_t chipNum = 1;
		
	//Host dependent peripheral initialized
	// First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
	/* Critical section callback */
	//	reg_wizchip_cris_cbfunc(ETH_SPI_CrisEnter, ETH_SPI_CrisExit);	//ע���ٽ�������
	/* Chip selection call back */
	reg_wizchip_cs_cbfunc(chipNum,ethConfig[1].SPI.CS._select, ethConfig[1].SPI.CS._deselect);  // CS must be tried with LOW
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(chipNum,ethConfig[1].SPI._read_byte, ethConfig[1].SPI._write_byte);	//ע���д����
	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(chipNum,CW_INIT_WIZCHIP,(void*)ethConfig[1].memSize) == -1) return -1;
	
	/* net set */
	ETH2_netSet();
	
	return 0;
}


/*
*********************************************************************************************************
*	�� �� ��: initEthHard
*	����˵��: W5500���ڳ�ʼ��
*	��    ��: 
*				ethID		����ID,�˵�·��ֻ��1�����������ethID=0
*				spiBaud  	SPI����ʱ��Ƶ�ʣ�֧�ֵĲ������£�
*							SPI_BAUD_100M    (0x00000000UL)	//100Mhz
*							SPI_BAUD_50M     (0x10000000UL)	//50Mhz
*							SPI_BAUD_25M     (0x20000000UL)	//25Mhz	
*							SPI_BAUD_12M5    (0x30000000UL)	//12.5Mhz
*							SPI_BAUD_6M25    (0x40000000UL)	//6.25Mhz                                                    
*             	ipAddr      Ҫ���õ�IP��ַָ��                         
*             	ipMask      Ҫ���õ���������ָ��
*		��		gwAddr 		Ҫ���õ�����ָ��
*            	mac         Ҫ���õ�mac��ַָ�룬���޸�mac��ַ�������ָ������ΪNULL
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t initEthHard(uint8_t ethID,uint32_t spiBaud,uint8_t* ipAddr,uint8_t* ipMask,uint8_t* gwAddr, uint8_t* mac)
{
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* ��ʼ��SPI ���� */
	// ethConfig[ethID].SPI._Init(spiBaud,SPI_PHASE_2EDGE, SPI_POLARITY_HIGH);
	ethConfig[ethID].SPI._Init();
	/* reset eth */
	ethConfig[ethID]._ethReset();
	
	/* ����������� */
	if(!ipAddr || !ipMask || !gwAddr) 
	{
		return -1;
	}
	else
	{
		memcpy(ethConfig[ethID].netInfo.ip,ipAddr,4);
		memcpy(ethConfig[ethID].netInfo.sn,ipMask,4);
		memcpy(ethConfig[ethID].netInfo.gw,gwAddr,4);
	}
	
	if(mac) 
	{
		memcpy(ethConfig[ethID].netInfo.mac,mac,6);
	}	
	
	if(ethConfig[ethID]._ethInit()==0) //W5500������������ԵȲ�����ʼ��
	{
		ethConfig[ethID].enable = 1;
		return 0;
	}	
	else return -1 ;
}
/*
*********************************************************************************************************
*	�� �� ��: modifyIP
*	����˵��: ����������Ϣ�޸�
*	��    ��: 
*				ethID		����ID,�˵�·��ֻ��1�����������ethID=0                                              
*             	ipAddr      Ҫ���õ�IP��ַָ��                         
*             	ipMask      Ҫ���õ���������ָ��
*		��		gwAddr 		Ҫ���õ�����ָ��
*            	mac         Ҫ���õ�mac��ַָ�룬���޸�mac��ַ�������ָ������ΪNULL
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t modifyIP(uint8_t ethID,uint8_t* ipAddr,uint8_t* ipMask,uint8_t* gwAddr, uint8_t* mac)
{
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	if(ethConfig[ethID].enable == 0) 
	{
		return -1;
	}
		
	/*�����������*/
	if(!ipAddr || !ipMask || !gwAddr) 
	{
		return -1;
	}
	else
	{
		memcpy(ethConfig[ethID].netInfo.ip,ipAddr,4);
		memcpy(ethConfig[ethID].netInfo.sn,ipMask,4);
		memcpy(ethConfig[ethID].netInfo.gw,gwAddr,4);
	}
	
	if(mac) 
	{
		memcpy(ethConfig[ethID].netInfo.mac,mac,6);
	}

	/* net set */
	ethConfig[ethID]._netSet();
	
	return 0;
}
/*
*********************************************************************************************************
*	�� �� ��: createTcpServerSocket
*	����˵��: ����Tcp Server Socket
*	��    ��: 
*				ethID			����ID,�˵�·��ֻ��1�����������ethID=0  
*				socketID		socket ID                                                
*             	serverPort      server Port                          
*		��		funcAddr 		Ӧ�ò�callback����
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t createTcpServerSocket(uint8_t ethID, uint16_t socketID,uint16_t serverPort,void * funcAddr)
{
	uint8_t chipNum;
	
	if(ethID >= Eth_Num)
	{
		return -1;
	}

	if(ethConfig[ethID].enable == 0) 
	{
		return -1;
	}
	
	chipNum = ethID;
	tcpsConfig[chipNum].socketID = socketID;
	tcpsConfig[chipNum].serverPort = serverPort;
	tcpsConfig[chipNum].funcAddr = (void (*)(uint8_t *, uint16_t, uint8_t *, uint16_t *))funcAddr;
	
	/*��socket*/
	socket(chipNum,socketID ,Sn_MR_TCP,serverPort,Sn_MR_ND);	
	
	/* enable TcpServerSocket */
	tcpsConfig[chipNum].enable = 1;
	
	return 0;
}
/*
*********************************************************************************************************
*	�� �� ��: createTcpClientSocket
*	����˵��: ����Tcp Client Socket
*	��    ��: 
*				ethID			����ID,�˵�·��ֻ��1�����������ethID=0  
*				socketID		socket ID                                                
*             	remotePort      remote Port                         
*             	remoteIp      	remote IP 
*             	localPort       local Port                         
*		��		funcAddr 		Ӧ�ò�callback����
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t createTcpClientSocket(uint8_t ethID, uint16_t socketID,uint16_t remotePort,uint8_t* remoteIp,uint16_t localPort,void * funcAddr) 
{
//	uint8_t chipNum;
	uint32_t aRandom32bit=0;
	
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	if(ethConfig[0].enable == 0) 
	{
		return -1;
	}
	
	rng_init();
	aRandom32bit = w5500_rng_get_random_num();
  tcp_port = W5500_TCP_ENSURE_LOCAL_PORT_RANGE(aRandom32bit);//(LWIP_RAND());
	
//	chipNum = ethID;
	tcpcConfig[ethID].socketID = socketID;
	tcpcConfig[ethID].remotePort = remotePort;
	tcpcConfig[ethID].localPort = tcp_port;
	memcpy(tcpcConfig[ethID].remoteIp,remoteIp,4);
	tcpcConfig[ethID].funcAddr = (void (*)(uint8_t *, uint16_t, uint8_t *, uint16_t *))funcAddr;
	printf("localPort = %d\r\n", tcp_port);
	/*��socket*/
	if(socket(0,socketID,Sn_MR_TCP,tcp_port,Sn_MR_ND) == socketID)
	{
		/* enable TcpClientSocket */
		tcpcConfig[ethID].enable = 1;
		return (uint8_t)socketID;
	}
	return 0;
}

int8_t w5500_close_socket(int8_t socket)
{
	if(socket<0)
		return 0;
	return close(0,socket);
}

uint32_t w5500_recv(uint8_t socket, uint8_t * buf)
{
	uint32_t len = getSn_RX_RSR(0, socket);			   
	if(len > 0)
	{
		recv(0, socket, buf, len);	
		return len;
	}
	return 0;
}

int8_t w5500_send(uint8_t socket, uint8_t * buf, uint16_t len)
{
	return SocketDataSend(0 , socket, buf, len);
}

/*
*********************************************************************************************************
*	�� �� ��: EtnHandle
*	����˵��: ����״̬��������
*	��    ��: 
*	
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t EtnHandle(uint8_t ethID)
{	
	uint8_t chipNum=0;
	static uint8_t bit = 0;

	if(ethID >= Eth_Num)
	{
		return -1;
	}
	
	/* ����δʹ�� */
	if(ethConfig[0].enable == 0) 
	{
		return -1;
	}
	
	if( (getPHYCFGR(chipNum) & 0x01) == 0 && bit)
	{
		bit = 0;
		close(chipNum,tcpcConfig[ethID].socketID);
	}

	/* Tcp Server Socket ��ʹ�� */
	if(tcpsConfig[ethID].enable == 1)
	{
		/* ��ȡsocket��״̬ */
		switch(getSn_SR(chipNum,tcpsConfig[ethID].socketID))									
		{
			/* socket���ڹر�״̬ */
			case SOCK_CLOSED:	
				/*������ܴ��ڵĳ�ʱ�жϱ�־*/ 	
				if (getSn_IR(chipNum,tcpsConfig[ethID].socketID) & Sn_IR_TIMEOUT)
				{
					IINCHIP_WRITE(chipNum,Sn_IR(tcpsConfig[ethID].socketID), (Sn_IR_TIMEOUT)); 
				}
				/* ��socket */
				socket(chipNum,tcpsConfig[ethID].socketID ,Sn_MR_TCP,tcpsConfig[ethID].serverPort,Sn_MR_ND);	  
				break;     
			
			/* socket�ѳ�ʼ��״̬ */
			case SOCK_INIT:	
				/* socket�������� */					
				listen(chipNum,tcpsConfig[ethID].socketID);								  
				break;
			
			/* socket�������ӽ���״̬ */
			case SOCK_ESTABLISHED:												          
				
				if(getSn_IR(chipNum,tcpsConfig[ethID].socketID) & Sn_IR_CON)
				{
					setSn_IR(chipNum,tcpsConfig[ethID].socketID, Sn_IR_CON);					/*��������жϱ�־λ*/
				}
					
				/* �鿴�Ƿ������� */
				tcpsConfig[ethID].inData.len = getSn_RX_RSR(chipNum, tcpsConfig[ethID].socketID);			   
				if(tcpsConfig[ethID].inData.len > 0)
				{
					/* ��������Client������ */
					recv(chipNum,tcpsConfig[ethID].socketID,tcpsConfig[ethID].inData.buff,tcpsConfig[ethID].inData.len);	
					/* ����Ӧ�ò����ݴ���callback���� */
					tcpsConfig[ethID].outData.len=0;
					memset(tcpsConfig[ethID].outData.buff,0,2048);
//					tcpsConfig[ethID].funcAddr(tcpsConfig[ethID].inData.buff,tcpsConfig[ethID].inData.len,tcpsConfig[ethID].outData.buff,&tcpsConfig[ethID].outData.len);
					if(tcpsConfig[ethID].outData.len > 0)
					{
						SocketDataSend(ethID , tcpsConfig[ethID].socketID,tcpsConfig[ethID].outData.buff,tcpsConfig[ethID].outData.len);
					}
				}
				break;
			
			case SOCK_CLOSE_WAIT:												            /*socket���ڵȴ��ر�״̬*/
				close(chipNum,tcpsConfig[ethID].socketID);
				break;
			
			default:
				break;
		}
	}
	
	/* Tcp Client Socket ��ʹ�� */
	if(tcpcConfig[ethID].enable == 1)
	{
		/* ��ȡsocket��״̬ */
		switch(getSn_SR(chipNum,tcpcConfig[ethID].socketID))								  	
		{
			/* socket���ڹر�״̬ */
			case SOCK_CLOSED:
				/*������ܴ��ڵĳ�ʱ�жϱ�־*/ 	
				if (getSn_IR(chipNum,tcpcConfig[ethID].socketID) & Sn_IR_TIMEOUT)
				{
					IINCHIP_WRITE(chipNum,Sn_IR(tcpcConfig[ethID].socketID), (Sn_IR_TIMEOUT)); 
				}			
				/* ��socket */
				socket(chipNum,tcpcConfig[ethID].socketID,Sn_MR_TCP,tcpcConfig[ethID].localPort,Sn_MR_ND);
				break;
			
			/* socket���ڳ�ʼ��״̬ */
			case SOCK_INIT:	
				/* socket���ӷ����� */ 					
				if(connect(chipNum,tcpcConfig[ethID].socketID,tcpcConfig[ethID].remoteIp,tcpcConfig[ethID].remotePort)==SOCK_OK)
				{					
					bit=1;
				}
				break;
			/* socket�������ӽ���״̬ */
			case SOCK_ESTABLISHED: 												           
				if(getSn_IR(chipNum,tcpcConfig[ethID].socketID) & Sn_IR_CON)
				{
					setSn_IR(chipNum,tcpcConfig[ethID].socketID, Sn_IR_CON); 					/* ��������жϱ�־λ */
					_ethernet_print();
				}
				return 1;	
			/* socket���ڵȴ��ر�״̬ */	
			case SOCK_CLOSE_WAIT: 											    	         
				close(chipNum,tcpcConfig[ethID].socketID);
				break;
				
			/* socket���������м�״̬ */				
			case SOCK_SYNSENT: 											    	         	
				break;
			
			default:
				break;
		}
	}
	return 0;
}
	
/*
*********************************************************************************************************
*	�� �� ��: SocketDataSend
*	����˵��: ͨ��socket�������ݣ�socket Ϊ����״̬���ܷ��ͳɹ�
*	��    ��: 
*				ethID			����ID,�˵�·��ֻ��1�����������ethID=0  
*				socketID		socket ID                                                
*             	outData      	�������ݵ�ָ��                         
*             	outLen      	�������ݵĳ���
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t SocketDataSend(uint8_t ethID, uint16_t socketID,uint8_t* outData,uint16_t outLen)
{
//	uint8_t chipNum;
	
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* ����δʹ�� */
	if(ethConfig[ethID].enable == 0) 
	{
		return -1;
	}
	
//	chipNum = ethID;


	/* socket Ϊ����״̬ */
	if(getSn_SR(0,socketID) == SOCK_ESTABLISHED )
	{
		int32_t sockret = 0;

		for(uint8_t i = 0;i < 5;i++)
		{
			sockret = send(ethID,socketID,outData,outLen);

			if(sockret != SOCK_BUSY)
				break;
		}

		if(sockret == outLen)  return 0;
		else return -1;
	}
	else
	{
		return -1;
	}
	
}

/*
*********************************************************************************************************
*	�� �� ��: SocketClose
*	����˵��: �ر�socket����ʧ��
*	��    ��: 
*				ethID			����ID,�˵�·��ֻ��1�����������ethID=0  
*				socketID		socket ID                                                
*
*	�� �� ֵ: 	0:�ɹ� -1��ʧ��
*********************************************************************************************************
*/
int8_t SocketClose(uint8_t ethID, uint16_t socketID)
{
	uint8_t chipNum;

	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* ����δʹ�� */
	if(ethConfig[ethID].enable == 0) 
	{
		return -1;
	}
	
	chipNum = ethID;

	if(socketID == tcpsConfig[ethID].socketID)
	{
		tcpsConfig[ethID].enable = 0;
		close(chipNum,socketID);
		return 0;
	}
	else if(socketID == tcpcConfig[ethID].socketID)
	{
		tcpcConfig[ethID].enable = 0;
		close(chipNum,socketID);
		return 0;
	}
	else return -1;
}

