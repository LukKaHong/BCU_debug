#include "DI.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
DI_Info_t DI_Info[DI_Signal_Max];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
DI_Info_t* GetDI_Info(DI_Signal_e di)
{
    if(di >= DI_Signal_Max)
        return NULL;

    return &DI_Info[di];
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void DI_Pro(uint8_t port)
{
    PortConfig_DI_t* DI = GetPortConfig_DI(port);
    if(DI == NULL)
        return;

    if(DI->en == 0)
        return;

    DI_Info_t* di_info = GetDI_Info(DI->signal);
    if(di_info == NULL)
        return;


    if(di_info->state == 0)
    {
        if(di_info->io == DI->valid)
        {
            if(++di_info->cnt >= DI->trigger_delay)
            {
                di_info->state = 1;
            }
        }
        else
        {
           di_info->cnt = 0; 
        }
    }
    else
    {
        if(di_info->io != DI->valid)
        {
            if(++di_info->cnt >= DI->recover_delay)
            {
                di_info->state = 0;
            }
        }
        else
        {
           di_info->cnt = 0; 
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void DI_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(DI_EventHandle, DI_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & DI_Event_Tick)
        {
            printf("%s\r\n", __func__);
        }
    }
}






