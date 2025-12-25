#include "AD.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"
#include "adc.h"
#include "NTCTable.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
NTC_Info_t NTC_Info[TEMP_Temp_Max];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
NTC_Info_t* GetNTC_Info(TEMP_Temp_e ntc)
{
    if(ntc >= TEMP_Temp_Max)
        return NULL;

    return &NTC_Info[ntc];
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint32_t Read_NTC_Res(uint8_t port)
{
    uint32_t TmpmV = 0;
    switch (port)
    {
    case 1:
        TmpmV =  (uint32_t)ADC_GetAverage_Channel(RT1_SAMP);
        break;
    case 2:
        TmpmV =  (uint32_t)ADC_GetAverage_Channel(RT2_SAMP);
    default:
        break;
    }

    TmpmV = (3300 * TmpmV / 65535);

    return (TmpmV * 10000) / (3300 - TmpmV);
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void TEMP_Pro(uint8_t port)
{
    PortConfig_TEMP_t* TEMP = GetPortConfig_TEMP(port);
    if(TEMP == NULL)
        return;

    if(TEMP->en == 0)
        return;

    NTC_Info_t* ntc_info = GetNTC_Info(TEMP->temp);
    if(ntc_info == NULL)
        return;

    ntc_info->temp = GetTemperatureFromNTCTable(Read_NTC_Res(port), TEMP->table);
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void ADC_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(ADC_EventHandle, ADC_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & ADC_Event_Tick)
        {
            for(uint8_t port = 1; port <= PortConfig_TEMP_Num; port++)
            {
                TEMP_Pro(port);
            }

            //printf("%s\r\n", __func__);
        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



