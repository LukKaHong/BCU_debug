#include "CommCAN_3.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_3_Task(void)
{
    uint32_t r_event;
    
    while(1)
    {
        r_event = osEventFlagsWait(CommCAN_3_EventHandle, CommCAN_3_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_3_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
    }
}
