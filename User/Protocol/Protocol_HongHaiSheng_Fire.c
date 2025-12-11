#include "PROTOCOL_HongHaiSheng_Fire.h"
#include "ProtocolNode.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CAN_Fire_HongHaiSheng_Recv(uint8_t device_no,CanMsgType *msg)
{
    if(device_no == 0 || device_no >= Fire_Num_Max)
        return;

    device_no -= 1; 

    switch(msg->id)
    {
        case 0x18FFBC01:
            if(0x00 == (msg->data[0] & 0x80))
            {
                *(GetNodePointer() + NODE_Fire_SENSOR_STATUS + device_no * Node_Num_Fire) = (msg->data[0] & 0x0f) - 1;
                *(GetNodePointer() + NODE_Fire_ALARM_STATUS + device_no * Node_Num_Fire)  = (msg->data[0] >> 4) & 0x03;
                
                *(GetNodePointer() + NODE_Fire_SENSOR_SMOKE_PHOTO_ALARM + device_no * Node_Num_Fire) = msg->data[1] & 0x03;
                *(GetNodePointer() + NODE_Fire_SENSOR_CO_ALARM + device_no * Node_Num_Fire)          = (msg->data[1] >> 2) & 0x03;
                *(GetNodePointer() + NODE_Fire_SENSOR_HIGH_TEMP_ALARM + device_no * Node_Num_Fire)   = (msg->data[1] >> 4) & 0x03;
                *(GetNodePointer() + NODE_Fire_SENSOR_LIQUID_LEAK_ALARM + device_no * Node_Num_Fire) = (msg->data[1] >> 6) & 0x03;
                
                int32_t temp = (uint16_t)((msg->data[4] << 8) | msg->data[3]);
                *(GetNodePointer() + NODE_Fire_TEMP_VALUE + device_no * Node_Num_Fire) = temp - 40 ;
                *(GetNodePointer() + NODE_Fire_CO_VALUE + device_no * Node_Num_Fire)   = (msg->data[6] << 8) | msg->data[5];
                
            }
            else
            {
                *(GetNodePointer() + NODE_Fire_SENSOR_STATUS + device_no * Node_Num_Fire)      = (msg->data[0] & 0x0f) - 1;
                *(GetNodePointer() + NODE_Fire_ALARM_STATUS + device_no * Node_Num_Fire)       = (msg->data[0] >> 4) & 0x03;

                *(GetNodePointer() + NODE_Fire_SMOKE_LIQUID_VALUE + device_no * Node_Num_Fire) = (msg->data[2] << 8) | msg->data[1];
                *(GetNodePointer() + NODE_Fire_SMOKE_PHOTO_VALUE + device_no * Node_Num_Fire)  = (uint16_t)(((msg->data[4] << 8) | msg->data[3]) * 0.0015);
            }
            break;
        default:
            break;
    }
}

