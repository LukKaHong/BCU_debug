#include "Protocol_n9_PCS.h"
#include "ProtocolNode.h"







/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CAN_PCS_n9_Recv(CAN_device_attr_t* device_attr, CanMsgType *msg)
{
    uint8_t device_no = device_attr->device_no;

    if(device_no == 0 || device_no >= PCS_Num_Max)
        return;

    device_no -= 1; 

    switch(CAN_ID_Deoffset_calc(msg->id, device_attr))
    {
        case 0x18100000:
            *(GetNodePointer() + NODE_PCS_V_AB + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_V_BC + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_V_CA + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            break;
        case 0x18110000:
            *(GetNodePointer() + NODE_PCS_V_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_V_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_V_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
			break;
		case 0x18120000:
            *(GetNodePointer() + NODE_PCS_I_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_I_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_I_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
			break;
		case 0x18130000:
            *(GetNodePointer() + NODE_PCS_IND_I_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_IND_I_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_IND_I_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
			break;
        case 0x18140000:
            *(GetNodePointer() + NODE_PCS_GRID_FREQ + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_GRID_PHASE_SEQ + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_DERATING_FACTOR + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_DERATING_FLAG + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18150000:
            *(GetNodePointer() + NODE_PCS_P_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_P_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_P_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_P_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18160000:
            *(GetNodePointer() + NODE_PCS_S_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_S_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_S_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_S_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18170000:
            *(GetNodePointer() + NODE_PCS_Q_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_Q_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_Q_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_Q_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18180000:
            *(GetNodePointer() + NODE_PCS_PF_A + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_PF_B + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_PF_C + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_PF_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18200000:
            *(GetNodePointer() + NODE_PCS_BUS_V_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_BUS_V_POS + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_BUS_V_NEG + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
			break;
        case 0x18210000:
            *(GetNodePointer() + NODE_PCS_BAT_V + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_BAT_I + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_DC_P + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
            *(GetNodePointer() + NODE_PCS_DC_I_TOTAL + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[6], msg->data[7]);
			break;
        case 0x18300000:
            *(GetNodePointer() + NODE_PCS_IGBT_TEMP + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[0], msg->data[1]);
            *(GetNodePointer() + NODE_PCS_ENV_TEMP + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[2], msg->data[3]);
            *(GetNodePointer() + NODE_PCS_IND_TEMP + device_no * Node_Num_PCS) = BUILD_S16_BA(msg->data[4], msg->data[5]);
			break;
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





