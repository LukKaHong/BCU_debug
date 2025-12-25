#ifndef  _BSP_Ethernet_H_
#define  _BSP_Ethernet_H_

#include "stm32h7xx_hal.h"
#include "bsp_wizchip_conf.h"

//定义网卡的数量
#define 	Eth_Num 		2 


/*Socket 端口选择，可按自己的习惯定义*/
#define SOCK_TCPS             0
#define SOCK_HUMTEM			 			0
#define SOCK_PING			  			0
#define SOCK_TCPC             1
#define SOCK_UDPS             2
#define SOCK_WEIBO      	  	2
#define SOCK_DHCP             3
#define SOCK_HTTPS            4
#define SOCK_DNS              5
#define SOCK_SMTP             6
#define SOCK_NTP              7


/** @defgroup SPI_BaudRate 
  * @{
  */
#define SPI_BAUD_100M               (0x00000000UL)	//100Mhz
#define SPI_BAUD_50M                (0x10000000UL)	//50Mhz
#define SPI_BAUD_25M                (0x20000000UL)	//25Mhz	
#define SPI_BAUD_12M5               (0x30000000UL)	//12.5Mhz
#define SPI_BAUD_6M25               (0x40000000UL)	//6.25Mhz


/* 网卡配置结构体 */
typedef struct ethConfig_t
{
	uint8_t	enable;				//网卡使能状态 1：使能  0：失能
	uint8_t ethID;				//w5500 网卡ID（1or2）

	struct
	{
		SPI_TypeDef * ID;
		uint32_t Baud;
		// void (*_Init)(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
		void (*_Init)(void);
		uint8_t (*_read_byte)   (void);
		void	(*_write_byte)  (uint8_t wb);
		struct
		{
			void (*_select)  (void);  
			void (*_deselect)(void);   
		}CS; 					
	}SPI;						//SPI配置
	
	wiz_NetInfo netInfo;		//网络信息设置
	
	uint8_t memSize[2][8]; 		//w5500 socket memery size 设置
	void (*_ethReset)(void);	//w5500 reset函数
	int8_t (*_ethInit)(void);	//w5500 initial函数
	void (*_netSet)(void);		//w5500 ip设置函数
	
}_ethConfig;

/* Tcp Server Socket配置结构体 */
typedef struct tcpserverConfig_t
{
	uint8_t		enable;			//socket使能状态 1：使能  0：失能
	uint8_t 	socketID;
	uint16_t 	serverPort;
	
	struct
	{	
		uint16_t len;
		uint8_t buff[2048];
	}inData;
	
	struct
	{	
		uint16_t len;
		uint8_t buff[2048];
	}outData;

	void (*funcAddr)(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen);
}_tcpserverConfig;

/* Tcp Client Socket配置结构体 */
typedef struct tcpclientConfig_t
{
	uint8_t		enable;			//使能状态 1：使能  0：失能
	uint8_t 	socketID;
	uint16_t 	remotePort;
	uint8_t 	remoteIp[4];
	uint16_t 	localPort;
	
	struct
	{	
		uint16_t len;
		uint8_t buff[1024 * 4];
	}inData;
	
	struct
	{	
		uint16_t len;
		uint8_t buff[1024 * 4];
	}outData;
	
	void (*funcAddr)(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen);
}_tcpclientConfig;


typedef struct
{	
	uint16_t len;
	uint8_t buff[2048];
}Eth_Data_TypeDef;


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
int8_t initEthHard(uint8_t ethID,uint32_t spiBaud,uint8_t* ipAddr,uint8_t* ipMask,uint8_t* gwAddr, uint8_t* mac);
/*
*********************************************************************************************************
*	函 数 名: modifyIP
*	功能说明: 网卡网络信息修改
*	形    参: 
*				ethID		网卡ID,0或1                                           
*             	ipAddr      要设置的IP地址指针                         
*             	ipMask      要设置的子网掩码指针
*		　		gwAddr 		要设置的网关指针
*            	mac         要设置的mac地址指针，不修改mac地址的情况下指针设置为NULL
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t modifyIP(uint8_t ethID,uint8_t* ipAddr,uint8_t* ipMask,uint8_t* gwAddr, uint8_t* mac);
/*
*********************************************************************************************************
*	函 数 名: createTcpServerSocket
*	功能说明: 创建Tcp Server Socket
*	形    参: 
*				ethID			网卡ID,0或1
*				socketID		socket ID                                                
*             	serverPort      server Port                         
*		　		funcAddr 		应用层callback函数
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t createTcpServerSocket(uint8_t ethID, uint16_t socketID,uint16_t serverPort,void * funcAddr);
/*
*********************************************************************************************************
*	函 数 名: createTcpClientSocket
*	功能说明: 创建Tcp Client Socket
*	形    参: 
*				ethID			网卡ID,0或1
*				socketID		socket ID                                                
*             	remotePort      remote Port                         
*             	remoteIp      	remote IP 
*             	localPort       local Port                         
*		　		funcAddr 		应用层callback函数
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
extern int8_t createTcpClientSocket(uint8_t ethID, uint16_t socketID,uint16_t remotePort,uint8_t* remoteIp,uint16_t localPort,void * funcAddr);
/*
*********************************************************************************************************
*	函 数 名: EtnHandle
*	功能说明: 网卡状态处理函数
*	形    参: 
*				ethID			网卡ID,0或1
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t EtnHandle(uint8_t ethID);
/*
*********************************************************************************************************
*	函 数 名: SocketDataSend
*	功能说明: 通过socket发送数据，socket 为连接状态才能发送成功
*	形    参: 
*				ethID			网卡ID,0或1
*				socketID		socket ID                                                
*             	outData      	发送数据的指针                         
*             	outLen      	发送数据的长度
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t SocketDataSend(uint8_t ethID, uint16_t socketID,uint8_t* outData,uint16_t outLen);

/*
*********************************************************************************************************
*	函 数 名: SocketClose
*	功能说明: 关闭socket，并失能
*	形    参: 
*				ethID			网卡ID,0或1
*				socketID		socket ID                                                
*
*	返 回 值: 	0:成功 -1：失败
*********************************************************************************************************
*/
int8_t SocketClose(uint8_t ethID, uint16_t socketID);

/*
*********************************************************************************************************
*	函 数 名: reg_ethernet_print_func
*	功能说明: 注册Ethernet连接成功后的初始信息打印函数
*	形    参: 
*				eth_print			初始信息打印函数
*	返 回 值: 	无
*********************************************************************************************************
*/
void reg_ethernet_print_func(void (*eth_print)(void));

int8_t w5500_close_socket(int8_t socket);
uint32_t w5500_recv(uint8_t socket, uint8_t * buf);
int8_t w5500_send(uint8_t socket, uint8_t * buf, uint16_t len);

extern _ethConfig ethConfig[Eth_Num];
extern _tcpserverConfig tcpsConfig[Eth_Num];	
extern _tcpclientConfig tcpcConfig[Eth_Num];

#endif
