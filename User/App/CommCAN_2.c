#include "CommCAN_2.h"
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
        printf("FDCAN2_Receive_Msg: ID=0x%X, Length=%d, Data=", CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex].id, CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex].length);
        for(int i = 0; i < CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex].length; i++)
        {
            printf("%02X ", CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex].data[i]);
        }
        printf("\n");

        Add_CAN_2_SendMsg(&CAN_2_ReceiveBuff.Msg[CAN_2_ReceiveBuff.CurIndex]);

        if(++CAN_2_ReceiveBuff.CurIndex >= CAN_ReceiveBuff_Max) CAN_2_ReceiveBuff.CurIndex = 0;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_2_Task(void)
{
    uint32_t r_event;

    while(1)
    {
        r_event = osEventFlagsWait(CommCAN_2_EventHandle, CommCAN_2_Event_Receive | CommCAN_2_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_2_Event_Receive)
        {
            CommCAN_2_Receive_Pro();
        }

        if(r_event & CommCAN_2_Event_Tick)
        {
            static CanMsgType msg;
            msg.id++;
            msg.length = 8;
            for(int i = 0; i < 8; i++)
            {
                msg.data[i] = i;
            }
            Add_CAN_2_SendMsg(&msg); 
        }
        
        CommCAN_2_Send_Pro();
    }
}
