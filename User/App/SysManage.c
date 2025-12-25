#include "SysManage.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"
#include "Led_Ctrl.h"
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
Device_Comm_t* GetDeviceCommPointer(void)
{
    return &Device_Comm;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
SysInfo_t* GetSysInfoPointer(void)
{
    return &Sys_info;
}

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
    Device_Comm.ECU.En      = 1;
    Device_Comm.ECU.Timeout = 10000;

    for(uint8_t i = 0; i < Sys_info.Air_Num; i++)
    {
        Device_Comm.Air[i].En      = 1;
        Device_Comm.Air[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.Air_Num; i++)
    {
        Device_Comm.Fire[i].En      = 1;
        Device_Comm.Fire[i].Timeout = 10000;
    }
    
    for(uint8_t i = 0; i < Sys_info.Meter_Num; i++)
    {
        Device_Comm.Meter[i].En      = 1;
        Device_Comm.Meter[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.Pv_Num; i++)
    {
        Device_Comm.Pv[i].En      = 1;
        Device_Comm.Pv[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.Doil_Num; i++)
    {
        Device_Comm.Doil[i].En      = 1;
        Device_Comm.Doil[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.Coolwater_Num; i++)
    {
        Device_Comm.Coolwater[i].En      = 1;
        Device_Comm.Coolwater[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.Dehum_Num; i++)
    {
        Device_Comm.Dehum[i].En      = 1;
        Device_Comm.Dehum[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.BMS_Num; i++)
    {
        Device_Comm.BMS[i].En      = 1;
        Device_Comm.BMS[i].Timeout = 10000;
    }

    for(uint8_t i = 0; i < Sys_info.PCS_Num; i++)
    {
        Device_Comm.PCS[i].En      = 1;
        Device_Comm.PCS[i].Timeout = 10000;
    }
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

void Cmd_PCS_GridOff(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_GRID_DISCONNECT + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_GRID_DISCONNECT + device_no * Node_Num_PCS] = 1;     
}

void Cmd_PCS_GridConnect(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_GRID_CONNECT + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_GRID_CONNECT + device_no * Node_Num_PCS] = 1;     
}

void Cmd_PCS_PowerOff(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_POWER_OFF + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_POWER_OFF + device_no * Node_Num_PCS] = 1; 
}

void Cmd_PCS_PowerOn(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_POWER_ON + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_POWER_ON + device_no * Node_Num_PCS] = 1;      
}

void Cmd_PCS_ResetFault(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_FAULT_RESET + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_FAULT_RESET + device_no * Node_Num_PCS] = 1;   
}

void Cmd_PCS_SetPower(uint8_t device_no, int16_t power)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no--;
    Write_Node_t* write_node = GetWriteNodePointer();

    write_node->writeflag[NODE_PCS_CMD_POWER_SET + device_no * Node_Num_PCS] = 1;
    write_node->value[NODE_PCS_CMD_POWER_SET + device_no * Node_Num_PCS] = power;
}

uint8_t Is_Clear_Fault_By_Poweroff(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return 0;

    device_no--;

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

uint8_t Is_Clear_Fault_By_Reset(uint8_t device_no)
{
    if(device_no == 0 || device_no >= PCS_Num_Max)
        return 0;

    device_no--;

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

void PCS_Allocating_power(int16_t power)
{
    if(power > 0)//charge
    {
        if(abs(power) > *(GetNodeValuePointer() + NODE_BMS_MAX_ALLOW_CHARGE_POWER))
            power = *(GetNodeValuePointer() + NODE_BMS_MAX_ALLOW_CHARGE_POWER);

        if(Sys_info.powerlimit0flag == 1)    
            power = 0;

        int16_t power_per_device = power / Sys_info.PCS_Num;
        for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
            Sys_info.setpower[device_no] = power_per_device;
    }
    else if(power < 0)//discharge
    {
        if(abs(power) > *(GetNodeValuePointer() + NODE_BMS_MAX_ALLOW_DISCHARGE_POWER))
            power = -(*(GetNodeValuePointer() + NODE_BMS_MAX_ALLOW_DISCHARGE_POWER));

        if(Sys_info.powerlimit0flag == 1)    
            power = 0;

        int16_t power_per_device = power / Sys_info.PCS_Num;
        for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
            Sys_info.setpower[device_no] = power_per_device;
    }
    else
    {
        for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
            Sys_info.setpower[device_no] = 0;
    }
}



void SysManage_PCS_Pro(void)
{
    // PCS_Allocating_power();

    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
    {
        if(device_no == 0 || device_no >= PCS_Num_Max)
            continue;

        switch (*(GetNodeValuePointer() + NODE_PCS_WORK_STATUS + device_no * Node_Num_PCS))
        {
        case 0: Sys_info.PCS_Status[device_no] = PCS_Status_stop;break;
        case 1:Sys_info.PCS_Status[device_no] = PCS_Status_standby;break;
        case 2:Sys_info.PCS_Status[device_no] = PCS_Status_run;break;
        case 3:Sys_info.PCS_Status[device_no] = PCS_Status_fault;break;
        default:continue;
        }

        if(Sys_info.Grid_Setting[device_no] == Grid_Setting_unkown)
            continue;

        if(Sys_info.Grid_Setting[device_no] != Sys_info.Grid_Status[device_no])
        {
            Cmd_PCS_PowerOff(device_no);
            Cmd_PCS_SetPower(device_no, 0);

            if(Sys_info.Grid_Setting[device_no] == Grid_Setting_Off)
                Cmd_PCS_GridOff(device_no);
            else if(Sys_info.Grid_Setting[device_no] == Grid_Setting_connect)
                Cmd_PCS_GridConnect(device_no);

            continue;
        }

        if(Sys_info.PCS_Status[device_no] == PCS_Status_standby)
        {
            if(Sys_info.PCS_Setting[device_no] == PCS_Status_stop)
            {
                Cmd_PCS_PowerOff(device_no);
            }
            else if(Sys_info.PCS_Setting[device_no] == PCS_Status_run)
            {
                Cmd_PCS_SetPower(device_no, Sys_info.setpower[device_no]);
            }
        }
        else if(Sys_info.PCS_Status[device_no] == PCS_Status_run)
        {
            if(Sys_info.PCS_Setting[device_no] == PCS_Status_stop)
            {
                Cmd_PCS_PowerOff(device_no);
                Cmd_PCS_SetPower(device_no, 0);
            }
            else if(Sys_info.PCS_Setting[device_no] == PCS_Status_run)
            {
                Cmd_PCS_SetPower(device_no, Sys_info.setpower[device_no]);
            }
        }
        else if(Sys_info.PCS_Status[device_no] == PCS_Status_stop)
        {
            if(Sys_info.PCS_Setting[device_no] == PCS_Status_run)
            {
                Cmd_PCS_PowerOn(device_no);
            }
        }
        else if(Sys_info.PCS_Status[device_no] == PCS_Status_fault)
        {
            if(Is_Clear_Fault_By_Poweroff(device_no))
            {
                Cmd_PCS_PowerOff(device_no);
                Cmd_PCS_SetPower(device_no, 0);
            }

            if(Is_Clear_Fault_By_Reset(device_no))
            {
                Cmd_PCS_PowerOff(device_no);
                Cmd_PCS_SetPower(device_no, 0);
                Cmd_PCS_ResetFault(device_no);
            }
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Pro_L0(void)
{
    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
        Sys_info.PCS_Setting[device_no] = PCS_Status_run;

    Sys_info.powerlimit0flag = 0;
}

void SysManage_Pro_L1_act1(void)
{
    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
        Sys_info.PCS_Setting[device_no] = PCS_Status_run;

    Sys_info.powerlimit0flag = 0;
}

void SysManage_Pro_L2_act1(void)
{
    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
        Sys_info.PCS_Setting[device_no] = PCS_Status_run;

    Sys_info.powerlimit0flag = 1;
}

void SysManage_Pro_L3_act1(void)
{
    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
        Sys_info.PCS_Setting[device_no] = PCS_Status_stop;

    Sys_info.powerlimit0flag = 0;
}

void SysManage_Pro_L4_act1(void)
{
    for(uint8_t device_no = 0; device_no < Sys_info.PCS_Num; device_no++)
        Sys_info.PCS_Setting[device_no] = PCS_Status_stop;

    Sys_info.powerlimit0flag = 0;
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
void SysManage_CalcDeviceNum(void)
{
    Sys_info.Air_Num = 0;
    Sys_info.Fire_Num = 0;
    Sys_info.Meter_Num = 0;
    Sys_info.Pv_Num = 0;
    Sys_info.Doil_Num = 0;
    Sys_info.Coolwater_Num = 0;
    Sys_info.Dehum_Num = 0;
    Sys_info.BMS_Num = 0;
    Sys_info.PCS_Num = 0;
    Sys_info.BMU_Num = 0;

    for(uint8_t i = 1; i <= PortConfig_rs485_Num; i++)
    {
        PortConfig_rs485_t* rs485 = GetPortConfig_rs485(i);
        if(rs485 == NULL)
            continue;;

        if(rs485->en == 0)
            continue;

        for(uint8_t j = 0; j < rs485->device_num; j++)
        {        
            if(rs485->device_attr[j].device_type == DEVICE_TYPE_Air)
                Sys_info.Air_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Fire)
                Sys_info.Fire_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Meter)
                Sys_info.Meter_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Pv)
                Sys_info.Pv_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Doil)
                Sys_info.Doil_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Coolwater)
                Sys_info.Coolwater_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_Dehum)
                Sys_info.Dehum_Num++;
            else if(rs485->device_attr[j].device_type == DEVICE_TYPE_PCS)
                Sys_info.PCS_Num++;
        }
    }

    for(uint8_t i = 1; i <= PortConfig_CAN_Num; i++)
    {
        PortConfig_CAN_t* can = GetPortConfig_CAN(i);
        if(can == NULL)
            return;

        if(can->en == 0)
            continue;

        for(uint8_t j = 0; j < can->device_num; j++)
        {        
            if(can->device_attr[j].device_type == DEVICE_TYPE_Air)
                Sys_info.Air_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Fire)
                Sys_info.Fire_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Meter)
                Sys_info.Meter_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Pv)
                Sys_info.Pv_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Doil)
                Sys_info.Doil_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Coolwater)
                Sys_info.Coolwater_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_Dehum)
                Sys_info.Dehum_Num++;
            else if(can->device_attr[j].device_type == DEVICE_TYPE_PCS)
                Sys_info.PCS_Num++;
        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_SelfCheck(void)
{

}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SysManage_Task(void)
{
    SysManage_Comm_Init();
    // Led_Init();

    while(1)
    {
        uint32_t r_event = osEventFlagsWait(SysManage_EventHandle, SysManage_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & SysManage_Event_Tick)
        {
            SysManage_Comm_Pro();
            SysManage_Fault_Pro();
            SysManage_Pro();
            SysManage_PCS_Pro();

            //printf("%s\r\n", __func__);
        }
    }
}












