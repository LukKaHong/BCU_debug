#include "CommCAN.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "ProtocolConvert.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm_CAN_Pro(uint8_t port, CanMsgType *msg)
{
    PortConfig_CAN_t* CAN = GetPortConfig_CAN(port);
    if(CAN == NULL)
        return;

    for(uint8_t device_num = 0; device_num < CAN->device_num; device_num++)//扫描所有设备
    {
        //获取协议
        ProtocolConvert_CAN_t* convert = GetProtocolConvert_CAN(CAN->device_attr[device_num].device_type);
        if(convert == NULL)
            continue;

        for(uint16_t node_num = 0; node_num < convert->node_num; node_num++)//扫描所有点表
        {
            if(convert->node_attr[node_num].frame_ID == CAN_ID_offset_calc(msg->id, &CAN->device_attr[device_num]))//匹配ID
            {
                ConvertToNode_CAN(GetNode(CAN->device_attr[device_num].device_type, CAN->device_attr[device_num].device_no), 
                                                msg->data,
                                                &convert->node_attr[node_num]);

                break;
            }
        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_1_Send_Pro(void)
{
    while(CAN_1_SendBuff.CurIndex != CAN_1_SendBuff.TxIndex)
    {
        if(FDCAN1_Send_Msg(&CAN_1_SendBuff.Msg[CAN_1_SendBuff.CurIndex]) != HAL_OK)
        {
            printf("FDCAN1_Send_Msg error\n");
            break;
        }

        if(++CAN_1_SendBuff.CurIndex >= CAN_SendBuff_Max) CAN_1_SendBuff.CurIndex = 0;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Add_CAN_1_SendMsg(CanMsgType *msg)
{
    CAN_1_SendBuff.Msg[CAN_1_SendBuff.TxIndex] = *msg;
    if(++CAN_1_SendBuff.TxIndex >= CAN_SendBuff_Max) CAN_1_SendBuff.TxIndex = 0;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_1_Receive_Pro(void)
{
    while(CAN_1_ReceiveBuff.CurIndex != CAN_1_ReceiveBuff.RxIndex)
    {
        Comm_CAN_Pro(1, &CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex]);

        if(++CAN_1_ReceiveBuff.CurIndex >= CAN_ReceiveBuff_Max) CAN_1_ReceiveBuff.CurIndex = 0;
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

void CommCAN_1_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommCAN_1_EventHandle, CommCAN_1_Event_Receive | CommCAN_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_1_Event_Receive)
        {
            CommCAN_1_Receive_Pro();
        }

        if(r_event & CommCAN_1_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
        
        CommCAN_1_Send_Pro();
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_2_Send_Pro(void)
{
    while(CAN_2_SendBuff.CurIndex != CAN_2_SendBuff.TxIndex)
    {
        if(FDCAN2_Send_Msg(&CAN_2_SendBuff.Msg[CAN_2_SendBuff.CurIndex]) != HAL_OK)
        {
            printf("FDCAN2_Send_Msg error\n");
            break;
        }

        CAN_2_SendBuff.CurIndex++;
        if(CAN_2_SendBuff.CurIndex >= CAN_SendBuff_Max) CAN_2_SendBuff.CurIndex = 0;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Add_CAN_2_SendMsg(CanMsgType *msg)
{
    CAN_2_SendBuff.Msg[CAN_2_SendBuff.TxIndex] = *msg;
    CAN_2_SendBuff.TxIndex++;
    if(CAN_2_SendBuff.TxIndex >= CAN_SendBuff_Max) CAN_2_SendBuff.TxIndex = 0;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_2_Receive_Pro(void)
{
    while(CAN_2_ReceiveBuff.CurIndex != CAN_2_ReceiveBuff.RxIndex)
    {
        Comm_CAN_Pro(2, &CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex]);

        if(++CAN_2_ReceiveBuff.CurIndex >= CAN_ReceiveBuff_Max) CAN_2_ReceiveBuff.CurIndex = 0;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_2_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommCAN_2_EventHandle, CommCAN_2_Event_Receive | CommCAN_2_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_2_Event_Receive)
        {
            CommCAN_2_Receive_Pro();
        }

        if(r_event & CommCAN_2_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
        
        CommCAN_2_Send_Pro();
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_3_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommCAN_3_EventHandle, CommCAN_3_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_3_Event_Tick)
        {
            printf("%s\r\n", __func__);

        }
    }
}




