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

void CommCAN_1_Task(void)
{
    CanMsgType msg;
    msg.id = 0x00;
    msg.length = 8;
    msg.data[0] = 0xf1;
    msg.data[1] = 0xf2;
    msg.data[2] = 0xf3;
    msg.data[3] = 0xf4;
    msg.data[4] = 0xf5;
    msg.data[5] = 0xf6;
    msg.data[6] = 0xf7;
    msg.data[7] = 0xf8;

    while(1)
    {
        osDelay(1000);

        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
        // Add_CAN_1_SendMsg(&msg);
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
