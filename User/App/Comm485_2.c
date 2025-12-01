#include "Comm485_2.h"
#include "cmsis_os.h"
// #include "ProtocolConvert.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_2_Tx_Buff[Uart_Tx_Buff_Size];
uint8_t Comm485_2_Rx_Buff[Uart_Rx_Buff_Size];
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_2_Task(void)
{
    // for(int i = 0; i < 1024; i++)
    // {
    //     Comm485_2_Tx_Buff[i] = i;
    // }

    uint32_t r_event;

    while(1)
    {
        r_event = osEventFlagsWait(Comm485_2_EventHandle, Comm485_2_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_2_Event_Tick)
        {
            printf("%s\r\n", __func__);
            // _485_2_Tx_And_Rx(Comm485_2_Tx_Buff, 20, Comm485_2_Rx_Buff, 1024);
        }
    }
}

