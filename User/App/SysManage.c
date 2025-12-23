#include "SysManage.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"


#include "PCF8523.h"
#include "W25Q128.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
Device_Comm_t Device_Comm;
SysInfo_t Sys_info;
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

    Sys_info.SysFaultLv = fault_lv;
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
void Cmd_BMS_PowerOn(void)
{
    
}

void Cmd_PCS_GridOff(void)
{
    
}

void Cmd_PCS_GridConnect(void)
{
    
}

void Cmd_PCS_PowerOff(void)
{
    
}

void Cmd_PCS_PowerOn(void)
{
    
}

void Cmd_PCS_ResetFault(void)
{
    
}

void Cmd_PCS_Power(int16_t power)
{

}

uint8_t Is_Clear_Fault_By_Poweroff(void)
{
    uint8_t device_no = 0;

    if(
        *(GetNodeValuePointer() + NODE_PCS_EPO_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_IGBT_HW_OVER_CURR_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BUS_HW_OVER_VOLT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_POWER_MOD_WAVE_LIMIT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_HW_OVER_CURR_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_24V_POWER_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_FAN_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_CONN_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_IND_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_OVER_TEMP_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_15V_POWER_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_SYS_FIRE_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BAT_DRY_CONTACT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_DRY_CONTACT_OVERLOAD_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_ENV_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_DRY_CONTACT_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS)
    )
        return 1;
    else
        return 0;
}

uint8_t Is_Clear_Fault_By_Reset(void)
{
    uint8_t device_no = 0;

    if(
        *(GetNodeValuePointer() + NODE_PCS_A_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_B_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_C_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_A_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_B_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_C_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_OVER_FREQ + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_UNDER_FREQ + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_PHASE_SEQ_ERR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_A_SW_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_B_SW_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_C_SW_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_VOLT_UNBALANCE + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_CURR_UNBALANCE + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_PHASE_LOSS + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_N_LINE_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_BUS_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_BUS_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_UNCTRL_RECT_BUS_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_UNCTRL_RECT_BUS_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_RUN_BUS_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_RUN_BUS_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_POS_NEG_BUS_UNBALANCE + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_CURR_MODE_BUS_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_DC_PRECHARGE_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_DC_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_SW_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_TIMEOUT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_A_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_B_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_C_OVER_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_CTRL_BOARD_EEPROM_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_AD_SAMPLE_ZERO_DRIFT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BACKEND_PROTO_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INSULATION_CHECK_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BMS_BAT_SYS_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_STS_COMM_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_BMS_COMM_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_EMS_COMM_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_CAN_COMM_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_CLOSE_FAIL + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_OPEN_FAIL + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_CLOSE_ERR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_OPEN_ERR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_CLOSE_FAIL + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_OPEN_FAIL + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_CLOSE_ERR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_OPEN_ERR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_AC_MAIN_RELAY_ADHESION + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_DC_RELAY_OPEN_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_A_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_B_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_C_OVER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_GRID_ISLAND_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_SYS_RESONANCE_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_SW_OVER_VOLT_CURR + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_HVRT_TIMEOUT_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_A_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_B_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_INV_V_C_UNDER_VOLT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_NO_SYNC_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_SHORT_FAULT + device_no * Node_Num_PCS) ||
        *(GetNodeValuePointer() + NODE_PCS_LVRT_TIMEOUT_FAULT + device_no * Node_Num_PCS)
    )
        return 1;
    else
        return 0;
}

void SysManage_PCS_Pro(void)
{
    if(Sys_info.Grid_Setting == Grid_Setting_unkown)
        return;

    if(Sys_info.Grid_Setting != Sys_info.Grid_Status)
    {
        Cmd_PCS_PowerOff();
        Cmd_PCS_Power(0);

        if(Sys_info.Grid_Setting == Grid_Setting_Off)
            Cmd_PCS_GridOff();
        else if(Sys_info.Grid_Setting == Grid_Setting_connect)
            Cmd_PCS_GridConnect();

        return;
    }

    if(Sys_info.PCS_Status == PCS_Status_standby)
    {
        if(Sys_info.PCS_Setting == PCS_Status_stop)
        {
            Cmd_PCS_PowerOff();
        }
        else if(Sys_info.PCS_Setting == PCS_Status_run)
        {
            Cmd_PCS_Power(Sys_info.power);
        }
    }
    else if(Sys_info.PCS_Status == PCS_Status_run)
    {
        if(Sys_info.PCS_Setting == PCS_Status_stop)
        {
            Cmd_PCS_PowerOff();
            Cmd_PCS_Power(0);
        }
        else if(Sys_info.PCS_Setting == PCS_Status_run)
        {
            Cmd_PCS_Power(Sys_info.power);
        }
    }
    else if(Sys_info.PCS_Status == PCS_Status_stop)
    {
        if(Sys_info.PCS_Setting == PCS_Status_run)
        {
            Cmd_PCS_PowerOn();
        }
    }
    else if(Sys_info.PCS_Status == PCS_Status_fault)
    {
        if(Is_Clear_Fault_By_Poweroff())
        {
            Cmd_PCS_PowerOff();
            Cmd_PCS_Power(0);
        }

        if(Is_Clear_Fault_By_Reset())
        {
            Cmd_PCS_PowerOff();
            Cmd_PCS_Power(0);
            Cmd_PCS_ResetFault();
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Pro_L0(void)
{

}

void SysManage_Pro_L1_act1(void)
{

}

void SysManage_Pro_L2_act1(void)
{

}

void SysManage_Pro_L3_act1(void)
{

}

void SysManage_Pro_L4_act1(void)
{

}

void SysManage_Pro(void)
{
    SysFaultConfig_t* faultconfig = GetSysFaultConfig();
    
    switch(Sys_info.SysFaultLv)
    {
        case SysFaultLv_0:
            SysManage_Pro_L0();
            break;
        case SysFaultLv_1:
            if(faultconfig->L1_action == 1)
                SysManage_Pro_L1_act1();
            break;
        case SysFaultLv_2:
            if(faultconfig->L2_action == 1)
                SysManage_Pro_L2_act1();
            break;
        case SysFaultLv_3:
            if(faultconfig->L3_action == 1)
                SysManage_Pro_L3_act1();
            break;
        case SysFaultLv_4:
            if(faultconfig->L4_action == 1)
                SysManage_Pro_L4_act1();
            break;
        default:
            break;
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
            SysManage_Comm_Pro();
            SysManage_Fault_Pro();


            SysManage_Pro();
            SysManage_PCS_Pro();

            printf("%s\r\n", __func__);
        }
    }
}












