#include "Protocol_n9_PCS.h"
#include "ProtocolNode.h"







/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CAN_PCS_n9_Recv(CAN_device_attr_t* device_attr, CanMsgType *msg)
{
    uint8_t device_no = device_attr->device_no;

    if(device_no == 0 || device_no > PCS_Num_Max)
        return;

    device_no -= 1; 

    switch(CAN_ID_Deoffset_calc(msg->id, device_attr))
    {
        case 0x18100000:
            *(GetNodeValuePointer() + NODE_PCS_V_AB + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_V_BC + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_V_CA + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
        case 0x18110000:
            *(GetNodeValuePointer() + NODE_PCS_V_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_V_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_V_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
		case 0x18120000:
            *(GetNodeValuePointer() + NODE_PCS_I_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_I_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_I_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
		case 0x18130000:
            *(GetNodeValuePointer() + NODE_PCS_IND_I_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_IND_I_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_IND_I_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
        case 0x18140000:
            *(GetNodeValuePointer() + NODE_PCS_GRID_FREQ + device_no * Node_Num_PCS)       = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_GRID_PHASE_SEQ + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_DERATING_FACTOR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_DERATING_FLAG + device_no * Node_Num_PCS)   = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18150000:
            *(GetNodeValuePointer() + NODE_PCS_P_A + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_P_B + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_P_C + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_P_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18160000:
            *(GetNodeValuePointer() + NODE_PCS_S_A + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_S_B + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_S_C + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_S_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18170000:
            *(GetNodeValuePointer() + NODE_PCS_Q_A + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_Q_B + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_Q_C + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_Q_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18180000:
            *(GetNodeValuePointer() + NODE_PCS_PF_A + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_PF_B + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_PF_C + device_no * Node_Num_PCS)     = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_PF_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18200000:
            *(GetNodeValuePointer() + NODE_PCS_BUS_V_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_BUS_V_POS + device_no * Node_Num_PCS)   = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_BUS_V_NEG + device_no * Node_Num_PCS)   = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
        case 0x18210000:
            *(GetNodeValuePointer() + NODE_PCS_BAT_V + device_no * Node_Num_PCS)      = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_BAT_I + device_no * Node_Num_PCS)      = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_DC_P + device_no * Node_Num_PCS)       = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_DC_I_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18300000:
            *(GetNodeValuePointer() + NODE_PCS_IGBT_TEMP + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_ENV_TEMP + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_IND_TEMP + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
        case 0x18400000:
            
            break;
        case 0x18500000:
            *(GetNodeValuePointer() + NODE_PCS_DC_CHARGE_HIST_H + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_DC_CHARGE_HIST_L + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_DC_CHARGE_DAY_H + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_DC_CHARGE_DAY_L + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18510000:
            *(GetNodeValuePointer() + NODE_PCS_DC_DISCHARGE_HIST_H + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_DC_DISCHARGE_HIST_L + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_DC_DISCHARGE_DAY_H + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_DC_DISCHARGE_DAY_L + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18520000:
            *(GetNodeValuePointer() + NODE_PCS_AC_CHARGE_HIST_H + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_AC_CHARGE_HIST_L + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_AC_CHARGE_DAY_H + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_AC_CHARGE_DAY_L + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18530000:
            *(GetNodeValuePointer() + NODE_PCS_AC_DISCHARGE_HIST_H + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodeValuePointer() + NODE_PCS_AC_DISCHARGE_HIST_L + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodeValuePointer() + NODE_PCS_AC_DISCHARGE_DAY_H + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodeValuePointer() + NODE_PCS_AC_DISCHARGE_DAY_L + device_no * Node_Num_PCS)  = BUILD_S16_BA(msg->data[6], msg->data[7]);
            break;
        case 0x18700000:
            {
                uint16_t temp = BUILD_U16_BA(msg->data[0], msg->data[1]);
                *(GetNodeValuePointer() + NODE_PCS_EPO_FAULT_FLAG + device_no * Node_Num_PCS)            = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_IGBT_HW_OVER_CURR_FLAG + device_no * Node_Num_PCS)    = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BUS_HW_OVER_VOLT_FLAG + device_no * Node_Num_PCS)     = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_POWER_MOD_WAVE_LIMIT_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 4)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_HW_OVER_CURR_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 5)) ? 1 : 0;

                temp = BUILD_U16_BA(msg->data[2], msg->data[3]);
                *(GetNodeValuePointer() + NODE_PCS_24V_POWER_FAULT_FLAG + device_no * Node_Num_PCS)             = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_FAN_FAULT_FLAG + device_no * Node_Num_PCS)                   = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_CONN_FAULT_FLAG + device_no * Node_Num_PCS)                  = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_LIGHTNING_ARRESTER_FAULT + device_no * Node_Num_PCS)         = (temp & (1 << 6)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_IND_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS)         = (temp & (1 << 7)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_POWER_MOD_OVER_TEMP_FLAG + device_no * Node_Num_PCS)         = (temp & (1 << 8)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_OVER_TEMP_FLAG + device_no * Node_Num_PCS)           = (temp & (1 << 9)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_15V_POWER_FAULT_FLAG + device_no * Node_Num_PCS)             = (temp & (1 << 10)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_SYS_FIRE_FAULT_FLAG + device_no * Node_Num_PCS)              = (temp & (1 << 11)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAT_DRY_CONTACT_FAULT_FLAG + device_no * Node_Num_PCS)       = (temp & (1 << 12)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_DRY_CONTACT_OVERLOAD_FAULT_FLAG + device_no * Node_Num_PCS)  = (temp & (1 << 13)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_ENV_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS)         = (temp & (1 << 14)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_DRY_CONTACT_OVER_TEMP_FAULT_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 15)) ? 1: 0;

                temp = BUILD_U16_BA(msg->data[4], msg->data[5]);
                *(GetNodeValuePointer() + NODE_PCS_A_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS)  = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_B_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS)  = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_C_OVER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS)  = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_A_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 3)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_B_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 4)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_C_UNDER_VOLT_FAULT_FLAG + device_no * Node_Num_PCS) = (temp & (1 << 5)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_OVER_FREQ + device_no * Node_Num_PCS)          = (temp & (1 << 6)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_UNDER_FREQ + device_no * Node_Num_PCS)         = (temp & (1 << 7)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_PHASE_SEQ_ERR + device_no * Node_Num_PCS)      = (temp & (1 << 8)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_A_SW_OVER_CURR + device_no * Node_Num_PCS)          = (temp & (1 << 9)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_B_SW_OVER_CURR + device_no * Node_Num_PCS)          = (temp & (1 << 10)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_C_SW_OVER_CURR + device_no * Node_Num_PCS)          = (temp & (1 << 11)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_VOLT_UNBALANCE + device_no * Node_Num_PCS)     = (temp & (1 << 12)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_CURR_UNBALANCE + device_no * Node_Num_PCS)     = (temp & (1 << 13)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_PHASE_LOSS + device_no * Node_Num_PCS)         = (temp & (1 << 14)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_N_LINE_OVER_CURR + device_no * Node_Num_PCS)        = (temp & (1 << 15)) ? 1: 0;

                temp = BUILD_U16_BA(msg->data[6], msg->data[7]);
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_BUS_OVER_VOLT + device_no * Node_Num_PCS)    = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_BUS_UNDER_VOLT + device_no * Node_Num_PCS)   = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_UNCTRL_RECT_BUS_OVER_VOLT + device_no * Node_Num_PCS)  = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_UNCTRL_RECT_BUS_UNDER_VOLT + device_no * Node_Num_PCS) = (temp & (1 << 3)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_RUN_BUS_OVER_VOLT + device_no * Node_Num_PCS)          = (temp & (1 << 4)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_RUN_BUS_UNDER_VOLT + device_no * Node_Num_PCS)         = (temp & (1 << 5)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_POS_NEG_BUS_UNBALANCE + device_no * Node_Num_PCS)      = (temp & (1 << 6)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAT_UNDER_VOLT + device_no * Node_Num_PCS)             = (temp & (1 << 7)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_CURR_MODE_BUS_UNDER_VOLT + device_no * Node_Num_PCS)   = (temp & (1 << 8)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAT_OVER_VOLT + device_no * Node_Num_PCS)              = (temp & (1 << 9)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_DC_PRECHARGE_OVER_CURR + device_no * Node_Num_PCS)     = (temp & (1 << 10)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_DC_OVER_CURR + device_no * Node_Num_PCS)               = (temp & (1 << 11)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAL_MOD_SW_OVER_CURR + device_no * Node_Num_PCS)       = (temp & (1 << 12)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BAT_REVERSE + device_no * Node_Num_PCS)                = (temp & (1 << 15)) ? 1: 0;
            }
            break;
        case 0x18710000:
            {
                uint16_t temp = BUILD_U16_BA(msg->data[0], msg->data[1]);
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_TIMEOUT + device_no * Node_Num_PCS)     = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_A_OVER_CURR + device_no * Node_Num_PCS) = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_B_OVER_CURR + device_no * Node_Num_PCS) = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_C_OVER_CURR + device_no * Node_Num_PCS) = (temp & (1 << 3)) ? 1: 0;

                temp = BUILD_U16_BA(msg->data[2], msg->data[3]);
                *(GetNodeValuePointer() + NODE_PCS_CTRL_BOARD_EEPROM_FAULT + device_no * Node_Num_PCS) = (temp & (1 << 1)) ? 1 : 0;
                *(GetNodeValuePointer() + NODE_PCS_AD_SAMPLE_ZERO_DRIFT + device_no * Node_Num_PCS)    = (temp & (1 << 2)) ? 1 : 0;
                *(GetNodeValuePointer() + NODE_PCS_BACKEND_PROTO_FAULT + device_no * Node_Num_PCS)     = (temp & (1 << 3)) ? 1 : 0;
                *(GetNodeValuePointer() + NODE_PCS_INSULATION_CHECK_FAULT + device_no * Node_Num_PCS)  = (temp & (1 << 8)) ? 1 : 0;
                *(GetNodeValuePointer() + NODE_PCS_BMS_BAT_SYS_FAULT + device_no * Node_Num_PCS)       = (temp & (1 << 11)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_STS_COMM_FAULT + device_no * Node_Num_PCS)          = (temp & (1 << 12)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_BMS_COMM_FAULT + device_no * Node_Num_PCS)          = (temp & (1 << 13)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_CAN_COMM_FAULT + device_no * Node_Num_PCS)          = (temp & (1 << 14)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_EMS_COMM_FAULT + device_no * Node_Num_PCS)          = (temp & (1 << 15)) ? 1: 0;
                
                temp = BUILD_U16_BA(msg->data[4], msg->data[5]);
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_CLOSE_FAIL + device_no * Node_Num_PCS) = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_OPEN_FAIL + device_no * Node_Num_PCS)  = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_CLOSE_ERR + device_no * Node_Num_PCS)  = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_PRECHARGE_RELAY_OPEN_ERR + device_no * Node_Num_PCS)   = (temp & (1 << 3)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_CLOSE_FAIL + device_no * Node_Num_PCS)      = (temp & (1 << 4)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_OPEN_FAIL + device_no * Node_Num_PCS)       = (temp & (1 << 5)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_CLOSE_ERR + device_no * Node_Num_PCS)       = (temp & (1 << 6)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_MAIN_RELAY_OPEN_ERR + device_no * Node_Num_PCS)        = (temp & (1 << 7)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_AC_MAIN_RELAY_ADHESION + device_no * Node_Num_PCS)     = (temp & (1 << 8)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_DC_RELAY_OPEN_FAULT + device_no * Node_Num_PCS)        = (temp & (1 << 9)) ? 1: 0;

                temp = BUILD_U16_BA(msg->data[6], msg->data[7]);
                *(GetNodeValuePointer() + NODE_PCS_INV_V_A_OVER_VOLT + device_no * Node_Num_PCS)      = (temp & (1 << 0)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_INV_V_B_OVER_VOLT + device_no * Node_Num_PCS)      = (temp & (1 << 1)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_INV_V_C_OVER_VOLT + device_no * Node_Num_PCS)      = (temp & (1 << 2)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_GRID_ISLAND_FAULT + device_no * Node_Num_PCS)      = (temp & (1 << 3)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_SYS_RESONANCE_FAULT + device_no * Node_Num_PCS)    = (temp & (1 << 5)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_SW_OVER_VOLT_CURR + device_no * Node_Num_PCS)      = (temp & (1 << 6)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_HVRT_TIMEOUT_FAULT + device_no * Node_Num_PCS)     = (temp & (1 << 8)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_INV_V_A_UNDER_VOLT + device_no * Node_Num_PCS)     = (temp & (1 << 9)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_INV_V_B_UNDER_VOLT + device_no * Node_Num_PCS)     = (temp & (1 << 10)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_INV_V_C_UNDER_VOLT + device_no * Node_Num_PCS)     = (temp & (1 << 11)) ? 1: 0;
                *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_NO_SYNC_FAULT + device_no * Node_Num_PCS) = (temp & (1 << 12)) ? 1: 0;   
                *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_SHORT_FAULT + device_no * Node_Num_PCS)   = (temp & (1 << 14)) ? 1: 0;   
                *(GetNodeValuePointer() + NODE_PCS_LVRT_TIMEOUT_FAULT + device_no * Node_Num_PCS)     = (temp & (1 << 15)) ? 1: 0;     
            }
            break;
        case 0x18f10000:
            if(0 == msg->data[0])
            {
                switch(msg->data[2] << 8 | msg->data[1])
                {
                    case 0x0d57:
                        break;
                    case 0x0d58:
                        *(GetNodeValuePointer() + NODE_PCS_REACTIVE_COMP_POWER_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x5066:
                        *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_MODE_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x501b:
                        *(GetNodeValuePointer() + NODE_PCS_MODULE_RUN_MODE_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x5060:
                        *(GetNodeValuePointer() + NODE_PCS_GRID_SINGLE_PHASE_CTRL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x5068:
                        *(GetNodeValuePointer() + NODE_PCS_MODULE_MASTER_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x5054:
                        *(GetNodeValuePointer() + NODE_PCS_MODULE_CURR_SOURCE_PARALLEL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x2227:
                        *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_VOLT_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x0103:
                        *(GetNodeValuePointer() + NODE_PCS_OFF_GRID_FREQ_SET + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02a6:
                        *(GetNodeValuePointer() + NODE_PCS_A_ACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02a7:
                        *(GetNodeValuePointer() + NODE_PCS_A_REACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02a8:
                        *(GetNodeValuePointer() + NODE_PCS_B_ACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02a9:
                        *(GetNodeValuePointer() + NODE_PCS_B_REACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02aa:
                        *(GetNodeValuePointer() + NODE_PCS_C_ACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x02ab:
                        *(GetNodeValuePointer() + NODE_PCS_C_REACTIVE_POWER_REF + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1604:
                        *(GetNodeValuePointer() + NODE_PCS_GRID_OVER_FREQ_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1605:
                        *(GetNodeValuePointer() + NODE_PCS_GRID_UNDER_FREQ_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1631:
                        *(GetNodeValuePointer() + NODE_PCS_ISLAND_OVER_FREQ_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1632:
                        *(GetNodeValuePointer() + NODE_PCS_ISLAND_UNDER_FREQ_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1633:
                        *(GetNodeValuePointer() + NODE_PCS_BAT_OVER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1634:
                        *(GetNodeValuePointer() + NODE_PCS_BAT_UNDER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1640:
                        *(GetNodeValuePointer() + NODE_PCS_GRID_MAX_CHARGE_DISCHARGE_CURR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1611:
                        *(GetNodeValuePointer() + NODE_PCS_BUS_OVER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1612:
                        *(GetNodeValuePointer() + NODE_PCS_BUS_UNDER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1641:
                        *(GetNodeValuePointer() + NODE_PCS_DC_OVER_CURR_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1600:
                        *(GetNodeValuePointer() + NODE_PCS_AC_LINE_OVER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1601:
                        *(GetNodeValuePointer() + NODE_PCS_AC_LINE_UNDER_VOLT_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1603:
                        *(GetNodeValuePointer() + NODE_PCS_AC_OVER_CURR_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1620:
                        *(GetNodeValuePointer() + NODE_PCS_MODULE_OVER_TEMP_PROT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1643:
                        *(GetNodeValuePointer() + NODE_PCS_CC_CV_SWITCH_VOLT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1644:
                        *(GetNodeValuePointer() + NODE_PCS_BAT_CV_CHARGE_VOLT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1650:
                        *(GetNodeValuePointer() + NODE_PCS_CV_CHARGE_MAX_TIME + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1651:
                        *(GetNodeValuePointer() + NODE_PCS_CV_FLOAT_SWITCH_CURR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1653:
                        *(GetNodeValuePointer() + NODE_PCS_BAT_FLOAT_VOLT + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1654:
                        *(GetNodeValuePointer() + NODE_PCS_FLOAT_CC_SWITCH_CURR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x0291:
                        break;
                    case 0x506b:
                        break;
                    case 0x1400:
                        break;
                    case 0x1405:
                        break;
                    case 0x1010:
                        *(GetNodeValuePointer() + NODE_PCS_FLOAT_CC_SWITCH_CURR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1007:
                        *(GetNodeValuePointer() + NODE_PCS_REMODBUS_BAUDRATE + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1011:
                        *(GetNodeValuePointer() + NODE_PCS_HMI_MODBUS_ADDR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1004:
                        *(GetNodeValuePointer() + NODE_PCS_HMI_MODBUS_BAUDRATE + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1648:
                        *(GetNodeValuePointer() + NODE_PCS_EMS_BMS_COMM_FAULT_TIME + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
                        break;
                    case 0x1020:
                        break;
                    case 0x1021:
                        break;
                    case 0x1022:
                        break;
                    case 0x1023:
                        break;
                    case 0x1024:
                        break;
                    case 0x1025:
                        break;
                    default:
                        break;
                }
            }
            break;;
        default:
            break;
    }



}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/





