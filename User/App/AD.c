#include "AD.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"
#include "adc.h"
#include "NTCTable.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
NTC_Info_t NTC_Info[NTC_Temp_Max];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
NTC_Info_t* GetNTC_Info(NTC_Temp_e ntc)
{
    if(ntc >= NTC_Temp_Max)
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
void NTC_Pro(uint8_t port)
{
    PortConfig_NTC_t* NTC = GetPortConfig_NTC(port);
    if(NTC == NULL)
        return;

    if(NTC->en == 0)
        return;

    NTC_Info_t* ntc_info = GetNTC_Info(NTC->temp);
    if(ntc_info == NULL)
        return;

    ntc_info->temp = GetTemperatureFromNTCTable(Read_NTC_Res(port), NTC->table);
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
            for(uint8_t port = 1; port <= PortConfig_NTC_Num; port++)
            {
                NTC_Pro(port);
            }

            // printf("%s\r\n", __func__);
        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



