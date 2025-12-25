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
GPIO_PinState Read_DI_Signal_Pin(uint8_t port)
{
    switch (port)
    {
    case 1:
        return HAL_GPIO_ReadPin(Switch_Detection1_GPIO_Port, Switch_Detection1_Pin);
    case 2:
        return HAL_GPIO_ReadPin(Switch_Detection2_GPIO_Port, Switch_Detection2_Pin);
    case 3:
        return HAL_GPIO_ReadPin(Switch_Detection3_GPIO_Port, Switch_Detection3_Pin);
    case 4:
        return HAL_GPIO_ReadPin(Switch_Detection4_GPIO_Port, Switch_Detection4_Pin);
    case 5:
        return HAL_GPIO_ReadPin(Switch_Detection5_GPIO_Port, Switch_Detection5_Pin);
    case 6:
        return HAL_GPIO_ReadPin(Switch_Detection6_GPIO_Port, Switch_Detection6_Pin);
    case 7:
        return HAL_GPIO_ReadPin(Switch_Detection7_GPIO_Port, Switch_Detection7_Pin);
    case 8:
        return HAL_GPIO_ReadPin(Switch_Detection8_GPIO_Port, Switch_Detection8_Pin);
    default:
        break;
    }

    return GPIO_PIN_RESET;
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void DI_Signal_Pro(uint8_t port)
{
    PortConfig_DI_t* DI = GetPortConfig_DI(port);
    if(DI == NULL)
        return;

    if(DI->en == 0)
        return;

    DI_Info_t* di_info = GetDI_Info(DI->signal);
    if(di_info == NULL)
        return;

    di_info->io = Read_DI_Signal_Pin(port);

    if(di_info->state == 0)
    {
        if(di_info->io == DI->valid)
        {
            if(++di_info->cnt >= DI->trigger_delay / DI_Task_Cycle)
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
            if(++di_info->cnt >= DI->recover_delay / DI_Task_Cycle)
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

            // DO_Ctrl(DO_Ctrl_red_led, 0);
            // DO_Ctrl(DO_Ctrl_green_led, 1);

            // HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
            // HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_SET);
            // HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_SET);

            for(uint8_t port = 1; port <= PortConfig_DI_Num; port++)
            {
                DI_Signal_Pro(port);
            }

            //printf("%s\r\n", __func__);
        }
    }
}






