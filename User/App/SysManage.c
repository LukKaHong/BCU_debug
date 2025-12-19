#include "SysManage.h"
#include "cmsis_os.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
Device_Comm_t Device_Comm;
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
void Comm_Fault_Pro(Comm_Fault_t* comm_fault)
{
    if(comm_fault->Rxflag == 1 || comm_fault->En == 0)
    {
        comm_fault->Cnt = 0;
        comm_fault->Fault = 0; 
    }
    else
    {
        if(++comm_fault->Cnt >= comm_fault->Timeout)
        {
            comm_fault->Cnt = comm_fault->Timeout;
            comm_fault->Fault = 1;
        }
    }

    comm_fault->Rxflag = 0;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Comm_Init(void)
{
    Device_Comm.Air[0].En      = 1;
    Device_Comm.Air[0].Timeout = 10000;

    Device_Comm.Fire[0].En      = 1;
    Device_Comm.Fire[0].Timeout = 10000;
    Device_Comm.Fire[1].En      = 1;
    Device_Comm.Fire[1].Timeout = 10000;

    Device_Comm.Meter[0].En      = 1;
    Device_Comm.Meter[0].Timeout = 10000;
    Device_Comm.Meter[1].En      = 1;
    Device_Comm.Meter[1].Timeout = 10000;

    Device_Comm.Pv[0].En      = 1;
    Device_Comm.Pv[0].Timeout = 10000;
    Device_Comm.Pv[1].En      = 1;
    Device_Comm.Pv[1].Timeout = 10000;

    Device_Comm.Doil[0].En      = 1;
    Device_Comm.Doil[0].Timeout = 10000;

    Device_Comm.Coolwater[0].En      = 1;
    Device_Comm.Coolwater[0].Timeout = 10000;

    Device_Comm.Dehum[0].En      = 1;
    Device_Comm.Dehum[0].Timeout = 10000;

    Device_Comm.BMS[0].En      = 1;
    Device_Comm.BMS[0].Timeout = 10000;

    Device_Comm.PCS[0].En      = 1;
    Device_Comm.PCS[0].Timeout = 10000;
    Device_Comm.PCS[1].En      = 1;
    Device_Comm.PCS[1].Timeout = 10000;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Comm_Pro(void)
{
    for(uint8_t i = 0; i < Air_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Air[i]);
    }

    for(uint8_t i = 0; i < Fire_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Fire[i]);
    }

    for(uint8_t i = 0; i < Meter_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Meter[i]);
    }

    for(uint8_t i = 0; i < Pv_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Pv[i]);
    }

    for(uint8_t i = 0; i < Doil_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Doil[i]);
    }

    for(uint8_t i = 0; i < Coolwater_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Coolwater[i]);
    }

    for(uint8_t i = 0; i < Dehum_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.Dehum[i]);
    }

    for(uint8_t i = 0; i < BMS_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.BMS[i]);
    }

    for(uint8_t i = 0; i < PCS_Num_Max; i++)
    {
        Comm_Fault_Pro(&Device_Comm.PCS[i]);
    }
}



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Task(void)
{
    SysManage_Comm_Init();
    
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












