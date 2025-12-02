#include "CommLAN_2.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommLAN_2_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommLAN_2_EventHandle, CommLAN_2_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommLAN_2_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
    }
}


