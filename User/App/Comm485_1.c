#include "Comm485_1.h"
#include "cmsis_os.h"
#include "usart.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_1_Rx_Buff[1024];
uint8_t Comm485_1_Tx_Buff[1024];
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_1_Task(void)
{
    // for(int i = 0; i < 1024; i++)
    // {
    //     Comm485_1_Tx_Buff[i] = i;
    // }
    uint32_t r_event;
    
    while(1)
    {
        r_event = osEventFlagsWait(Comm485_1_EventHandle, Comm485_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_1_Event_Tick)
        {
            printf("%s\r\n", __func__);
            // _485_1_Tx_And_Rx(Comm485_1_Tx_Buff, 30, Comm485_1_Rx_Buff, 1024);
        }
    }
}






