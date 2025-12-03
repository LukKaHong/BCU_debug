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
#if 1
char json_string[] = "{\n"
"    \"modbus\": [\n"
"        {\n"
"            \"port\": 1,\n"
"            \"baud\": 9600,\n"
"            \"date_bit\": 8,\n"
"            \"stop_bit\": 1,\n"
"            \"parity\": 0,\n"
"            \"device_num\": 1,\n"
"            \"device_attr\": [\n"
"                {\n"
"                    \"device_type\": 1,\n"
"                    \"device_no\": 1,\n"
"                    \"device_addr\": 1\n"
"                }\n"
"            ]\n"
"        }\n"
"    ],\n"
"    \"CAN\": [\n"
"        {\n"
"            \"port\": 1,\n"
"            \"baud\": 500000,\n"
"            \"device_num\": 1,\n"
"            \"device_attr\": [\n"
"                {\n"
"                    \"device_type\": 1,\n"
"                    \"device_no\": 1,\n"
"                    \"master_addr\": 1,\n"
"                    \"slave_addr\": 1,\n"
"                    \"addr_format\": 0\n"
"                }\n"
"            ]\n"
"        }\n"
"    ]\n"
"}";

#endif

void Comm485_1_Task(void)
{
    cJSON_To_PortConfig(json_string);

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






