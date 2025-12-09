#include "ProtocolNode.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
Node_PCS_t Node_PCS[PCS_Num_Max];
Node_Air_t Node_Air[Air_Num_Max];
Node_Meter_t Node_Meter[Meter_Num_Max];
Node_Fire_t Node_Fire[Fire_Num_Max];
Node_Doil_t Node_Doil[Doil_Num_Max];
Node_Pv_t Node_Pv[Pv_Num_Max];
Node_Coolwater_t Node_Coolwater[Coolwater_Num_Max];
Node_Dehum_t Node_Dehum[Dehum_Num_Max];
// Node_ECU_t Node_ECU[ECU_Num_Max];
// Node_BMS_t Node_BMS[BMS_Num_Max];
// Node_BMU_t Node_BMU[BMU_Num_Max];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint16_t* GetNode(DEVICE_TYPE_e device_type, uint8_t no)
{
    if(no == 0)
        return NULL;

    switch (device_type)
    {
    case DEVICE_TYPE_PCS:
        if(no > PCS_Num_Max)
            return NULL;
        return &Node_PCS[no - 1].Node[0];
    case DEVICE_TYPE_Air:
        if(no > Air_Num_Max)
            return NULL;
        return &Node_Air[no - 1].Node[0];
    case DEVICE_TYPE_Meter:
        if(no > Meter_Num_Max)
            return NULL;
        return &Node_Meter[no - 1].Node[0];
    case DEVICE_TYPE_Fire:
        if(no > Fire_Num_Max)
            return NULL;
        return &Node_Fire[no - 1].Node[0];
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return NULL;
        return &Node_Doil[no - 1].Node[0];
    case DEVICE_TYPE_Pv:
        if(no > Pv_Num_Max)
            return NULL;
        return &Node_Pv[no - 1].Node[0];
    case DEVICE_TYPE_Coolwater:
        if(no > Coolwater_Num_Max)
            return NULL;
        return &Node_Coolwater[no - 1].Node[0];
    case DEVICE_TYPE_Dehum:
        if(no > Dehum_Num_Max)
            return NULL;
        return &Node_Dehum[no - 1].Node[0];
    default:
        return NULL;
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

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/




