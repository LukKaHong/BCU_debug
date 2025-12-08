#include "Comm485_1.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"

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
    printf("modbus_node_attr_t : %d\r\n", sizeof(modbus_node_attr_t));
    printf("CAN_node_attr_t : %d\r\n", sizeof(CAN_node_attr_t));

    printf("ProtocolConvert_modbus_t : %d\r\n", sizeof(ProtocolConvert_modbus_t));
    printf("ProtocolConvert_CAN_t : %d\r\n", sizeof(ProtocolConvert_CAN_t));

    while(1)
    {
        uint32_t r_event = osEventFlagsWait(Comm485_1_EventHandle, Comm485_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_1_Event_Tick)
        {
            // printf("%s\r\n", __func__);
            // Comm_485_Pro(1, Comm485_1_Tx_Buff, Comm485_1_Rx_Buff);
        }
    }
}






