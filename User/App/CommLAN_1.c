#include "CommLAN_1.h"
#include "cmsis_os.h"
#include "ModbusTCP.h"
#include "ModbusTCP_Slave.h"
#include "ProtocolNode.h"



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Receiv_LAN_EMS(uint8_t* rx, uint16_t rxlen, uint8_t* tx, uint16_t* txlen)
{
    int32_t ret = ModbusTCP_Slave_Handle(rx, rxlen, 1, 
        GetNodePointer(), Node_Num_Max, 
        tx, txlen);
        
    if(ret == MBTCP_OK)
    {
        // 发送响应
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommLAN_1_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommLAN_1_EventHandle, CommLAN_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommLAN_1_Event_Tick)
        {
            // printf("%s\r\n", __func__);
        }
    }
}

