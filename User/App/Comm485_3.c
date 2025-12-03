#include "Comm485_3.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_3_Rx_Buff[Uart_Rx_Buff_Size];
uint8_t Comm485_3_Tx_Buff[Uart_Tx_Buff_Size];
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_3_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(Comm485_3_EventHandle, Comm485_3_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_3_Event_Tick)
        {
            // printf("%s\r\n", __func__);
            // Comm_485_Pro(3, Comm485_3_Tx_Buff, Comm485_3_Rx_Buff);
        }
    }
}


