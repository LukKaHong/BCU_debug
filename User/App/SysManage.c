#include "SysManage.h"
#include "cmsis_os.h"
#include "ProtocolNode.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Fault_Pro(void)
{
    Node_t* node = GetNodePointer();
    uint16_t fault_lv = 0;

    for(uint16_t i = 0; i < Node_Num_Max; i++)
    {
        if(node->type[i] == NODE_TYPE_fault)
        {
            if(node->value[i] > fault_lv)
                fault_lv = node->value[i];
        }
    }
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
            SysManage_Fault_Pro();

            printf("%s\r\n", __func__);
        }
    }
}












