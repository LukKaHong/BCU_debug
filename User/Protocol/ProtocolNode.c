#include "ProtocolNode.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static uint16_t Node[Node_Num_Max];
static Write_Node_t Write_Node;

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint16_t* GetNodePointer(void)
{
    return Node;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
Write_Node_t* GetWriteNodePointer(void)
{
    return &Write_Node;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint16_t* GetNode(DEVICE_TYPE_e device_type, uint8_t no, uint16_t model_id)
{
    if(no == 0)
        return NULL;

    switch (device_type)
    {
    case DEVICE_TYPE_PCS:
        if(no > PCS_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_PCS];
    case DEVICE_TYPE_Air:
        if(no > Air_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Air];
    case DEVICE_TYPE_Meter:
        if(no > Meter_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Meter];
    case DEVICE_TYPE_Fire:
        if(no > Fire_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Fire];
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Doil];
    case DEVICE_TYPE_Pv:
        if(no > Pv_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Pv];
    case DEVICE_TYPE_Coolwater:
        if(no > Coolwater_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Coolwater];
    case DEVICE_TYPE_Dehum:
        if(no > Dehum_Num_Max)
            return NULL;
        return &Node[model_id + (no - 1) * Node_Num_Dehum];
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




