#include "CommLAN_1.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommLAN_1_Task(void)
{
    uint32_t r_event;
    
    while(1)
    {
        r_event = osEventFlagsWait(CommLAN_1_EventHandle, CommLAN_1_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommLAN_1_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
    }
}

