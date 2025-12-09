#include "CommLAN_1.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Receiv_LAN_1(uint8_t* data, uint32_t len)
{



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

