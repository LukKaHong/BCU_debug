#include "SysManage.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Fault_Pro(void)
{


    
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(SysManage_EventHandle, SysManage_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & SysManage_Event_Tick)
        {


            printf("%s\r\n", __func__);
        }
    }
}












