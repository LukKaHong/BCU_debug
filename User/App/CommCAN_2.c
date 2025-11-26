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
void CommCAN_2_Task(void)
{
    CanMsgType msg;
    msg.id = 0x00;
    msg.length = 8;
    msg.data[0] = 0x08;
    msg.data[1] = 0x07;
    msg.data[2] = 0x06;
    msg.data[3] = 0x05;
    msg.data[4] = 0x04;
    msg.data[5] = 0x03;
    msg.data[6] = 0x02;
    msg.data[7] = 0x01;

    while(1)
    {
        osDelay(1000);

        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        // Add_CAN_2_SendMsg(&msg);
        CommCAN_2_Send_Pro();

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
