#include "CommLAN_1.h"
#include "cmsis_os.h"
#include "ModbusTCP.h"
#include "ModbusTCP_Slave.h"
#include "ProtocolNode.h"
#include "wiz_interface.h"
#include "bsp_ethernet.h"
#include "LAN_ZVPP_ECSUpper.h"
#include "LAN_ZVPP_ECSUpper_IF.h"
#include "task.h"
#include "main.h"

static int ECSSock,UpperSock;
static uint16_t LAN_Upper_Timeout_Cnt = 0;
static uint16_t LAN_ECS_Timeout_Cnt = 0;
static uint16_t LAN_Timeout_Cnt = 0;
static uint16_t Handshake_Timeout_Cnt = 0;
static tLANinfo LANinfo;
static volatile uint8_t SNAck = 0;
tPowerTest PowerTest;
uint8_t System_Reset_Flag = 0;

#define SN_ACK_SUCCESS 0xAAAA
#define	LANTASKPERIOD CommLAN_1_Task_Cycle
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void app_eth1_handle(uint8_t* inData,uint16_t inLen,uint8_t* outData,uint16_t *outLen) //回调仅用于测试网口
{
	if(NULL == inData)
	return;
	if(0 == inLen)
	return;
	if(NULL == outData)
	return;
	
	*outLen = inLen;
	memcpy(outData,inData,inLen);
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void LAN_ZVPP_ECSUpper_Init(void)
{
	ECSSock = -1;
	UpperSock = -1;

	//W5500网口初始化
	initEthHard(0,SPI_BAUD_12M5,ethConfig[0].netInfo.ip,ethConfig[0].netInfo.sn,ethConfig[0].netInfo.gw, ethConfig[0].netInfo.mac);
	createTcpClientSocket(0, tcpcConfig[0].socketID, tcpcConfig[0].remotePort, tcpcConfig[0].remoteIp,tcpcConfig[0].localPort,app_eth1_handle); 
	createTcpClientSocket(1, tcpcConfig[1].socketID, tcpcConfig[1].remotePort, tcpcConfig[1].remoteIp,tcpcConfig[1].localPort,app_eth1_handle);
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/
void CloseAllSocket(void)
{
#if LAN_W5500
	w5500_close_socket(UpperSock);
	w5500_close_socket(ECSSock);
#endif
#if LAN_LWIP
	closesocket(UpperSock);
	closesocket(ECSSock);
#endif
	
	UpperSock = -1;
	ECSSock = -1;		
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/

int Is_LAN_ECS_Connect(void)
{
	if(++LAN_ECS_Timeout_Cnt >= (10 * 1000 / LANTASKPERIOD))//超时n秒没有心跳包
	{
		LAN_ECS_Timeout_Cnt = 0;
		return -1;
	}
	
	return 0;
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/
int Is_LAN_Upper_Connect(void)
{
	if(++LAN_Upper_Timeout_Cnt >= (10 * 1000 / LANTASKPERIOD))//超时n秒没有心跳包
	{
		LAN_Upper_Timeout_Cnt = 0;
		return -1;
	}
	
	return 0;
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/
int Is_LAN_Connect(void)
{
	if(++LAN_Timeout_Cnt >= (20 * 1000 / LANTASKPERIOD))//超时n秒没有心跳包
	{
		LAN_Timeout_Cnt = 0;
		return -1;
	}
	
	return 0;
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/
int Is_Handshake_Timeout(void)
{
	if(++Handshake_Timeout_Cnt >= (10))//超时n秒没有心跳包
	{
		Handshake_Timeout_Cnt = 0;
		return -1;
	}
	
	return 0;
}
/*
-----------------------------------------------------------------------------------

-----------------------------------------------------------------------------------
*/
static uint8_t LAN_W5500_ECSUpper_SendRecv(int sock)
{
	return 0;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void LAN_W5500_Pro(void)
{
	if(0 == LANinfo.connect)//没有连接，先尝试连接ECS
	{
		if(System_Reset_Flag == 1)
			return;

		if(1 == EtnHandle(0))
		{
			//连接上了ECS
			LANinfo.connect = 1;
			ECSSock = 1;
			LAN_Timeout_Cnt = 0;
			printf("LAN Connect To ECS\r\n");
			return;	
		}
		else
		{
			w5500_close_socket(ECSSock);
			ECSSock = -1;
			vTaskDelay(10);
			if(-1 == Is_LAN_Connect())
			{
				LANinfo.connect = 3;
				return;
			}
		}
		if(1 == EtnHandle(1))
		{
			//连接上了上位机
			LANinfo.connect = 2;
			UpperSock = 2;
			LAN_Timeout_Cnt = 0;
			printf("LAN Connect To Upper\r\n");
			return;
		}
		else
		{
			w5500_close_socket(UpperSock);
			UpperSock = -1;
			vTaskDelay(10);
			if(-1 == Is_LAN_Connect())
			{
				LANinfo.connect = 3;
				return;
			}
		}
	}
	else if(1 == LANinfo.connect) //ECS
	{
		//超时没有收到任何数据
		if(-1 == Is_LAN_ECS_Connect())
		{
			w5500_close_socket(ECSSock);
			ECSSock = -1;
			SNAck = 0;
			LANinfo.connect = 0;

			printf("LAN ECS Timeout\r\n");
			return;
		}
		if(1 == LAN_W5500_ECSUpper_SendRecv(ECSSock))
		{
			w5500_close_socket(ECSSock);
			ECSSock = -1;
			SNAck = 0;
			LANinfo.connect = 0;

			printf("LAN ECS Disconnect\r\n");
		}
	}
	else if(2 == LANinfo.connect) //上位机
	{
		//超时没有收到任何数据
		if(-1 == Is_LAN_Upper_Connect())
		{
			w5500_close_socket(UpperSock);
			UpperSock = -1;	
			SNAck = 0;
			LANinfo.connect = 0;

			printf("LAN Upper Timeout\r\n");
			return;
		}
		if(1 == LAN_W5500_ECSUpper_SendRecv(UpperSock))
		{
			w5500_close_socket(UpperSock);
			UpperSock = -1;	
			SNAck = 0;
			LANinfo.connect = 0;

			printf("LAN Upper Disconnect\r\n");
		}
	}
	else if(3 == LANinfo.connect)  //重置网口、DHCP服务
	{
		LAN_ZVPP_ECSUpper_Init();
		LANinfo.connect = 0;
	}
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Receiv_LAN_EMS(uint8_t* rx, uint16_t rxlen, uint8_t* tx, uint16_t* txlen)
{
    int32_t ret = ModbusTCP_Slave_Handle(rx, rxlen, 1, 
        GetNodeValuePointer(), Node_Num_Max, 
        tx, txlen,
        GetWriteNodePointer()
    );
        
    if(ret == MBTCP_OK)
    {
        // 发送响应
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommLAN_1_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommLAN_1_EventHandle, CommLAN_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommLAN_1_Event_Tick)
        {
            //printf("%s\r\n", __func__);
        }
				
		LAN_W5500_Pro();
        // SPI_ReadWriteByte(6, 0xaa);
    }
}

