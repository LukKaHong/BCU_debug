/*
*********************************************************************************************************
*
*	模块名称 : 以太网接口驱动程序
*	文件名称 : bsp_ethernet.c
*	版    本 : V1.0
*	说    明 : 以太网接口驱动程序,双W5500
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.1    2021-11-22  wr     正式发布
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

/*定义eth网卡信息，如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
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
										.netInfo.dhcp =  NETINFO_DHCP,
										
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

/* Tcp Server Socket配置结构体初始化 */
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
/* Tcp Client Socket配置结构体初始化 */				
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
*@brief		client连接server成功后的初始信息打印函数
*@param		初始信息打印函数直指针
*@return	无
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

wiz_NetTimeout sWIZNetTimeout = {.time_100us = 0x64, .retry_cnt = 5};           /* 超时0x64*0.1ms 重试 5次; 芯片默认为超时200ms，重试8次 */


/**
 * @brief Default function to handle in tcpserver or tcpclient.
 * @note This function help not to access wrong address. If you do not describe this function or register any functions,
 * null function is called.
 */
void defaultHandle(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen) {};
	
/**
  * @brief  进入临界区
  * @retval None
  */
void ETH_SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}
/**
  * @brief  退出临界区
  * @retval None
  */
void ETH_SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
  * @brief  从SPI总线发送1字节数据
  * @retval 读到的数据
  */
void ETH1_SPI_WriteByte(uint8_t TxData)
{			
	bsp_spiw55002TransferByte(TxData);
}

/**
  * @brief  从SPI总线读取1字节数据
  * @retval 读到的数据
  */
uint8_t ETH1_SPI_ReadByte(void)
{		
	return(bsp_spiw55002TransferByte(0x00)); 		 	
}

/**
  * @brief  片选信号输出低电平
  * @retval None
  */
void ETH1_SPI_CS_Select(void)
{
	 HAL_GPIO_WritePin(A_W5500_SCSn_GPIO_Port, A_W5500_SCSn_Pin, GPIO_PIN_RESET);
}
/**
  * @brief  片选信号输出高电平
  * @retval None
  */
void ETH1_SPI_CS_Deselect(void)
{
	 HAL_GPIO_WritePin(A_W5500_SCSn_GPIO_Port, A_W5500_SCSn_Pin, GPIO_PIN_SET);
}

/**
  * @brief  复位W5500芯片
  * @retval None
  */
void ETH1_RESET(void)
{
	HAL_GPIO_WritePin(A_W5500_RSTn_GPIO_Port, A_W5500_RSTn_Pin, GPIO_PIN_RESET); /*复位管脚输出低电平 */
//	bsp_DelayMS(5);
	delay_ms(5);
	HAL_GPIO_WritePin(A_W5500_RSTn_GPIO_Port, A_W5500_RSTn_Pin, GPIO_PIN_SET); 	/*复位管脚输出高电平 */
//	bsp_DelayMS(50);
	delay_ms(50);
}

/*
*********************************************************************************************************
*	函 数 名: ETH1_netSet
*	功能说明: 网口1网络设置(IP、mask、gate等网络信息)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
#define ETHERNET_BUF_MAX_SIZE (1024 * 2)
uint8_t ethernet_buf[ETHERNET_BUF_MAX_SIZE] = {0};
void ETH1_netSet(void)
{
	uint8_t chipNum = 0;

	/* Network initialization */
	// ctlnetwork(chipNum, CN_SET_NETINFO, (void*)&(ethConfig[0].netInfo));
	network_init(chipNum, ethernet_buf, (void*)&(ethConfig[0].netInfo));
	ctlnetwork(chipNum, CN_GET_NETINFO, (void*)&(ethConfig[0].netInfo));

	/*调整w5500的超时时间，默认值为超时200ms，重试8次，没有需要可以不调整*/
	//ctlnetwork(CN_SET_TIMEOUT, (void*)&sWIZNetTimeout);

	/* 5*3秒keep alive数据发送不通，则重启socket */
	setRTR(chipNum,5);									/* 超时时间 */
	WIZCHIP_WRITE(chipNum, Sn_KPALVTR(SOCK_TCPS),0x03);	/*打开keep alive模式*/
	WIZCHIP_WRITE(chipNum, Sn_KPALVTR(SOCK_TCPC),0x03);	/*打开keep alive模式*/	
	
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
*	函 数 名: ETH1_Init
*	功能说明: 网口1初始化函数
*	形    参：无
*	返 回 值: 0：成功 -1：失败
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
	//	reg_wizchip_cris_cbfunc(ETH_SPI_CrisEnter, ETH_SPI_CrisExit);	//注册临界区函数
	/* Chip selection call back */
	reg_wizchip_cs_cbfunc(chipNum,ethConfig[0].SPI.CS._select, ethConfig[0].SPI.CS._deselect);  // CS must be tried with LOW
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(chipNum,ethConfig[0].SPI._read_byte, ethConfig[0].SPI._write_byte);	//注册读写函数
	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(chipNum,CW_INIT_WIZCHIP,(void*)ethConfig[0].memSize) == -1) return -1;
	
	/* net set */
	ETH1_netSet();
	
	return 0;
}

/**
  * @brief  从SPI总线发送1字节数据
  * @retval 读到的数据
  */
void ETH2_SPI_WriteByte(uint8_t TxData)
{			
	bsp_spiw55002TransferByte(TxData);
}

/**
  * @brief  从SPI总线读取1字节数据
  * @retval 读到的数据
  */
uint8_t ETH2_SPI_ReadByte(void)
{		
	return(bsp_spiw55002TransferByte(0x00)); 		    
}

/**
  * @brief  片选信号输出低电平
  * @retval None
  */
void ETH2_SPI_CS_Select(void)
{
//	 HAL_GPIO_WritePin(SPI5_SCS_PORT, SPI5_SCS_PIN, GPIO_PIN_RESET);
}
/**
  * @brief  片选信号输出高电平
  * @retval None
  */
void ETH2_SPI_CS_Deselect(void)
{
//	 HAL_GPIO_WritePin(SPI5_SCS_PORT, SPI5_SCS_PIN, GPIO_PIN_SET);
}

/**
  * @brief  复位W5500芯片
  * @retval None
  */
void ETH2_RESET(void)
{
//	HAL_GPIO_WritePin(SPI5_RST_PORT, SPI5_RST_PIN, GPIO_PIN_RESET); /*复位管脚输出低电平 */
////	bsp_DelayMS(5);
//	delay_ms(5);
//	HAL_GPIO_WritePin(SPI5_RST_PORT, SPI5_RST_PIN, GPIO_PIN_SET); 	/*复位管脚输出高电平 */
////	bsp_DelayMS(50);
//	delay_ms(50);
}

/*
*********************************************************************************************************
*	函 数 名: ETH1_netSet
*	功能说明: 网口1网络设置(IP、mask、gate等网络信息)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ETH2_netSet(void)
{
	uint8_t chipNum = 1;

	/* Network initialization */
	ctlnetwork(chipNum, CN_SET_NETINFO, (void*)&(ethConfig[1].netInfo));
	ctlnetwork(chipNum, CN_GET_NETINFO, (void*)&(ethConfig[1].netInfo));

	/*调整w5500的超时时间，默认值为超时200ms，重试8次，没有需要可以不调整*/
	//ctlnetwork(CN_SET_TIMEOUT, (void*)&sWIZNetTimeout);

	/* 5*3秒keep alive数据发送不通，则重启socket */
	setRTR(chipNum,5);									/* 超时时间 */
	IINCHIP_WRITE(chipNum,Sn_KPALVTR(SOCK_TCPS),0x03);	/*打开keep alive模式*/
	IINCHIP_WRITE(chipNum,Sn_KPALVTR(SOCK_TCPC),0x03);	/*打开keep alive模式*/	
	
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
*	函 数 名: ETH1_Init
*	功能说明: 网口1初始化函数
*	形    参：无
*	返 回 值: 0：成功 -1：失败
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
	//	reg_wizchip_cris_cbfunc(ETH_SPI_CrisEnter, ETH_SPI_CrisExit);	//注册临界区函数
	/* Chip selection call back */
	reg_wizchip_cs_cbfunc(chipNum,ethConfig[1].SPI.CS._select, ethConfig[1].SPI.CS._deselect);  // CS must be tried with LOW
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(chipNum,ethConfig[1].SPI._read_byte, ethConfig[1].SPI._write_byte);	//注册读写函数
	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(chipNum,CW_INIT_WIZCHIP,(void*)ethConfig[1].memSize) == -1) return -1;
	
	/* net set */
	ETH2_netSet();
	
	return 0;
}


/*
*********************************************************************************************************
*	函 数 名: initEthHard
*	功能说明: W5500网口初始化
*	形    参: 
*				ethID		网卡ID,此电路板只有1个网卡，因此ethID=0
*				spiBaud  	SPI总线时钟频率，支持的参数如下：
*							SPI_BAUD_100M    (0x00000000UL)	//100Mhz
*							SPI_BAUD_50M     (0x10000000UL)	//50Mhz
*							SPI_BAUD_25M     (0x20000000UL)	//25Mhz	
*							SPI_BAUD_12M5    (0x30000000UL)	//12.5Mhz
*							SPI_BAUD_6M25    (0x40000000UL)	//6.25Mhz                                                    
*             	ipAddr      要设置的IP地址指针                         
*             	ipMask      要设置的子网掩码指针
*		　		gwAddr 		要设置的网关指针
*            	mac         要设置的mac地址指针，不修改mac地址的情况下指针设置为NULL
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t initEthHard(uint8_t ethID,uint32_t spiBaud,uint8_t* ipAddr,uint8_t* ipMask,uint8_t* gwAddr, uint8_t* mac)
{
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* 初始化SPI 总线 */
	// ethConfig[ethID].SPI._Init(spiBaud,SPI_PHASE_2EDGE, SPI_POLARITY_HIGH);
	ethConfig[ethID].SPI._Init();
	/* reset eth */
	ethConfig[ethID]._ethReset();
	
	/* 网络参数设置 */
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
	
	if(ethConfig[ethID]._ethInit()==0) //W5500网络参数、重试等参数初始化
	{
		ethConfig[ethID].enable = 1;
		return 0;
	}	
	else return -1 ;
}
/*
*********************************************************************************************************
*	函 数 名: modifyIP
*	功能说明: 网卡网络信息修改
*	形    参: 
*				ethID		网卡ID,此电路板只有1个网卡，因此ethID=0                                              
*             	ipAddr      要设置的IP地址指针                         
*             	ipMask      要设置的子网掩码指针
*		　		gwAddr 		要设置的网关指针
*            	mac         要设置的mac地址指针，不修改mac地址的情况下指针设置为NULL
*
*	返 回 值: 	0:成功 -1：失败
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
		
	/*网络参数设置*/
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
*	函 数 名: createTcpServerSocket
*	功能说明: 创建Tcp Server Socket
*	形    参: 
*				ethID			网卡ID,此电路板只有1个网卡，因此ethID=0  
*				socketID		socket ID                                                
*             	serverPort      server Port                          
*		　		funcAddr 		应用层callback函数
*
*	返 回 值: 	0:成功 -1：失败
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
	
	/*打开socket*/
	socket(chipNum,socketID ,Sn_MR_TCP,serverPort,Sn_MR_ND);	
	
	/* enable TcpServerSocket */
	tcpsConfig[chipNum].enable = 1;
	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: createTcpClientSocket
*	功能说明: 创建Tcp Client Socket
*	形    参: 
*				ethID			网卡ID,此电路板只有1个网卡，因此ethID=0  
*				socketID		socket ID                                                
*             	remotePort      remote Port                         
*             	remoteIp      	remote IP 
*             	localPort       local Port                         
*		　		funcAddr 		应用层callback函数
*
*	返 回 值: 	0:成功 -1：失败
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
	/*打开socket*/
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
*	函 数 名: EtnHandle
*	功能说明: 网卡状态处理函数
*	形    参: 
*	
*	返 回 值: 	0:成功 -1：失败
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
	
	/* 网卡未使能 */
	if(ethConfig[0].enable == 0) 
	{
		return -1;
	}
	
	if( (getPHYCFGR(chipNum) & 0x01) == 0 && bit)
	{
		bit = 0;
		close(chipNum,tcpcConfig[ethID].socketID);
	}

	/* Tcp Server Socket 已使能 */
	if(tcpsConfig[ethID].enable == 1)
	{
		/* 获取socket的状态 */
		switch(getSn_SR(chipNum,tcpsConfig[ethID].socketID))									
		{
			/* socket处于关闭状态 */
			case SOCK_CLOSED:	
				/*清除可能存在的超时中断标志*/ 	
				if (getSn_IR(chipNum,tcpsConfig[ethID].socketID) & Sn_IR_TIMEOUT)
				{
					IINCHIP_WRITE(chipNum,Sn_IR(tcpsConfig[ethID].socketID), (Sn_IR_TIMEOUT)); 
				}
				/* 打开socket */
				socket(chipNum,tcpsConfig[ethID].socketID ,Sn_MR_TCP,tcpsConfig[ethID].serverPort,Sn_MR_ND);	  
				break;     
			
			/* socket已初始化状态 */
			case SOCK_INIT:	
				/* socket建立监听 */					
				listen(chipNum,tcpsConfig[ethID].socketID);								  
				break;
			
			/* socket处于连接建立状态 */
			case SOCK_ESTABLISHED:												          
				
				if(getSn_IR(chipNum,tcpsConfig[ethID].socketID) & Sn_IR_CON)
				{
					setSn_IR(chipNum,tcpsConfig[ethID].socketID, Sn_IR_CON);					/*清除接收中断标志位*/
				}
					
				/* 查看是否有数据 */
				tcpsConfig[ethID].inData.len = getSn_RX_RSR(chipNum, tcpsConfig[ethID].socketID);			   
				if(tcpsConfig[ethID].inData.len > 0)
				{
					/* 接收来自Client的数据 */
					recv(chipNum,tcpsConfig[ethID].socketID,tcpsConfig[ethID].inData.buff,tcpsConfig[ethID].inData.len);	
					/* 调用应用层数据处理callback函数 */
					tcpsConfig[ethID].outData.len=0;
					memset(tcpsConfig[ethID].outData.buff,0,2048);
//					tcpsConfig[ethID].funcAddr(tcpsConfig[ethID].inData.buff,tcpsConfig[ethID].inData.len,tcpsConfig[ethID].outData.buff,&tcpsConfig[ethID].outData.len);
					if(tcpsConfig[ethID].outData.len > 0)
					{
						SocketDataSend(ethID , tcpsConfig[ethID].socketID,tcpsConfig[ethID].outData.buff,tcpsConfig[ethID].outData.len);
					}
				}
				break;
			
			case SOCK_CLOSE_WAIT:												            /*socket处于等待关闭状态*/
				close(chipNum,tcpsConfig[ethID].socketID);
				break;
			
			default:
				break;
		}
	}
	
	/* Tcp Client Socket 已使能 */
	if(tcpcConfig[ethID].enable == 1)
	{
		/* 获取socket的状态 */
		switch(getSn_SR(chipNum,tcpcConfig[ethID].socketID))								  	
		{
			/* socket处于关闭状态 */
			case SOCK_CLOSED:
				/*清除可能存在的超时中断标志*/ 	
				if (getSn_IR(chipNum,tcpcConfig[ethID].socketID) & Sn_IR_TIMEOUT)
				{
					IINCHIP_WRITE(chipNum,Sn_IR(tcpcConfig[ethID].socketID), (Sn_IR_TIMEOUT)); 
				}			
				/* 打开socket */
				socket(chipNum,tcpcConfig[ethID].socketID,Sn_MR_TCP,tcpcConfig[ethID].localPort,Sn_MR_ND);
				break;
			
			/* socket处于初始化状态 */
			case SOCK_INIT:	
				/* socket连接服务器 */ 					
				if(connect(chipNum,tcpcConfig[ethID].socketID,tcpcConfig[ethID].remoteIp,tcpcConfig[ethID].remotePort)==SOCK_OK)
				{					
					bit=1;
				}
				break;
			/* socket处于连接建立状态 */
			case SOCK_ESTABLISHED: 												           
				if(getSn_IR(chipNum,tcpcConfig[ethID].socketID) & Sn_IR_CON)
				{
					setSn_IR(chipNum,tcpcConfig[ethID].socketID, Sn_IR_CON); 					/* 清除接收中断标志位 */
					_ethernet_print();
				}
				return 1;	
			/* socket处于等待关闭状态 */	
			case SOCK_CLOSE_WAIT: 											    	         
				close(chipNum,tcpcConfig[ethID].socketID);
				break;
				
			/* socket处于连接中间状态 */				
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
*	函 数 名: SocketDataSend
*	功能说明: 通过socket发送数据，socket 为连接状态才能发送成功
*	形    参: 
*				ethID			网卡ID,此电路板只有1个网卡，因此ethID=0  
*				socketID		socket ID                                                
*             	outData      	发送数据的指针                         
*             	outLen      	发送数据的长度
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t SocketDataSend(uint8_t ethID, uint16_t socketID,uint8_t* outData,uint16_t outLen)
{
//	uint8_t chipNum;
	
	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* 网卡未使能 */
	if(ethConfig[ethID].enable == 0) 
	{
		return -1;
	}
	
//	chipNum = ethID;


	/* socket 为连接状态 */
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
*	函 数 名: SocketClose
*	功能说明: 关闭socket，并失能
*	形    参: 
*				ethID			网卡ID,此电路板只有1个网卡，因此ethID=0  
*				socketID		socket ID                                                
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t SocketClose(uint8_t ethID, uint16_t socketID)
{
	uint8_t chipNum;

	if(ethID >= Eth_Num)
	{
		return -1;
	}	
	
	/* 网卡未使能 */
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

