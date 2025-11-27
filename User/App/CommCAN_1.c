#include "CommCAN_1.h"
#include "cmsis_os.h"
#include "fdcan.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

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
        printf("FDCAN1_Receive_Msg: ID=0x%X, Length=%d, Data=", CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex].id, CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex].length);
        for(int i = 0; i < CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex].length; i++)
        {
            printf("%02X ", CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex].data[i]);
        }
        printf("\n");

        Add_CAN_1_SendMsg(&CAN_1_ReceiveBuff.Msg[CAN_1_ReceiveBuff.CurIndex]);

        if(++CAN_1_ReceiveBuff.CurIndex >= CAN_ReceiveBuff_Max) CAN_1_ReceiveBuff.CurIndex = 0;
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

void CommCAN_1_Task(void)
{
    uint32_t r_event;

    while(1)
    {
        r_event = osEventFlagsWait(CommCAN_1_EventHandle, CommCAN_1_Event_Receive | CommCAN_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_1_Event_Receive)
        {
            CommCAN_1_Receive_Pro();
        }

        if(r_event & CommCAN_1_Event_Tick)
        {
            printf("%s\r\n", __func__);
            
            static CanMsgType msg;
            msg.id++;
            msg.length = 8;
            for(int i = 0; i < 8; i++)
            {
                msg.data[i] = i;
            }
            Add_CAN_1_SendMsg(&msg);
        }
        
        CommCAN_1_Send_Pro();
    }
}
