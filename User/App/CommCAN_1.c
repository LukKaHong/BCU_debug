#include "CommCAN_1.h"
#include "cmsis_os.h"
#include "fdcan.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
tCAN_SendBuff CAN_1_SendBuff;
tCAN_ReceiveBuff CAN_1_ReceiveBuff;
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
            printf("FDCAN1_Send_Msg error\r\n");
            break;
        }

        CAN_1_SendBuff.CurIndex++;
        if(CAN_1_SendBuff.CurIndex >= CAN_SendBuff_Max) CAN_1_SendBuff.CurIndex = 0;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Add_CAN_1_SendMsg(CanMsgType *msg)
{
    CAN_1_SendBuff.Msg[CAN_1_SendBuff.TxIndex] = *msg;
    CAN_1_SendBuff.TxIndex++;
    if(CAN_1_SendBuff.TxIndex >= CAN_SendBuff_Max) CAN_1_SendBuff.TxIndex = 0;
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

void CommCAN_1_Task(void)
{
    CanMsgType msg;
    msg.id = 0x00;
    msg.length = 8;
    msg.data[0] = 0x01;
    msg.data[1] = 0x02;
    msg.data[2] = 0x03;
    msg.data[3] = 0x04;
    msg.data[4] = 0x05;
    msg.data[5] = 0x06;
    msg.data[6] = 0x07;
    msg.data[7] = 0x08;

    while(1)
    {
        osDelay(1000);

        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        Add_CAN_1_SendMsg(&msg);
        CommCAN_1_Send_Pro();

        msg.data[0]++;
        msg.data[1]++;
        msg.data[2]++;
        msg.data[3]++;
        msg.data[4]++;
        msg.data[5]++;
        msg.data[6]++;
        msg.data[7]++;
    }
}
