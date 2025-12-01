#include "Comm485_1.h"
#include "cmsis_os.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_1_Rx_Buff[Uart_Rx_Buff_Size];
uint8_t Comm485_1_Tx_Buff[Uart_Tx_Buff_Size];
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






