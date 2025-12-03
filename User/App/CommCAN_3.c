#include "CommCAN_3.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "ProtocolConvert.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommCAN_3_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommCAN_3_EventHandle, CommCAN_3_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommCAN_3_Event_Tick)
        {
            // printf("%s\r\n", __func__);
        }
    }
}
