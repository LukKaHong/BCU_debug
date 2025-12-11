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
reg_addr<-->id<-->node_index

index = id + (no - 1) * offset;
id = index - (no - 1) * offset;
----------------------------------------------------------------------------------------------
*/
bool NodeIndexToModelId(DEVICE_TYPE_e device_type, uint8_t no, uint16_t index, uint16_t* model_id)
{
    if(no == 0 || device_type >= DEVICE_TYPE_Max)
        return false;

    switch (device_type)
    {
    case DEVICE_TYPE_PCS:
        if(no > PCS_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_PCS;
        return true;
    case DEVICE_TYPE_Air:
        if(no > Air_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Air;
        return true;
    case DEVICE_TYPE_Meter:
        if(no > Meter_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Meter;
        return true;
    case DEVICE_TYPE_Fire:
        if(no > Fire_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Fire;
        return true;
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Doil;
        return true;
    case DEVICE_TYPE_Pv:
        if(no > Pv_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Pv;
        return true;
    case DEVICE_TYPE_Coolwater:
        if(no > Coolwater_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Coolwater;
        return true;
    case DEVICE_TYPE_Dehum:
        if(no > Dehum_Num_Max)
            return false;
        *model_id = index - (no - 1) * Node_Num_Dehum;
        return true;
    default:
        return false;
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
bool ModelIdToRegAddr_modbus(DEVICE_TYPE_e device_type, uint16_t model_id, uint16_t* reg_addr, uint8_t* fun_code)
{
    ProtocolConvert_modbus_t* convert = GetProtocolConvert_modbus(device_type);
    if(convert == NULL)
        return false;

    for(uint16_t i = 0; i < convert->node_num; i++)
    {
        if(convert->node_attr[i].model_id == model_id)
        {
            if(reg_addr != NULL) *reg_addr = convert->node_attr[i].reg_addr;
            if(fun_code != NULL) *fun_code = convert->node_attr[i].fun_code;

            return true;
        }
    }

    return false;
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
bool ModelIdToNodeIndex(DEVICE_TYPE_e device_type, uint8_t no, uint16_t model_id, uint16_t* index)
{
    if(no == 0 || device_type >= DEVICE_TYPE_Max)
        return false;

    switch (device_type)
    {
    case DEVICE_TYPE_PCS:
        if(no > PCS_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_PCS;
        return true;
    case DEVICE_TYPE_Air:
        if(no > Air_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Air;
        return true;
    case DEVICE_TYPE_Meter:
        if(no > Meter_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Meter;
        return true;
    case DEVICE_TYPE_Fire:
        if(no > Fire_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Fire;
        return true;
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Doil;
        return true;
    case DEVICE_TYPE_Pv:
        if(no > Pv_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Pv;
        return true;
    case DEVICE_TYPE_Coolwater:
        if(no > Coolwater_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Coolwater;
        return true;
    case DEVICE_TYPE_Dehum:
        if(no > Dehum_Num_Max)
            return false;
        *index = model_id + (no - 1) * Node_Num_Dehum;
        return true;
    default:
        return false;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
bool GetNodeRange(DEVICE_TYPE_e device_type, uint8_t no,uint16_t* start, uint16_t* end)
{
    if(no == 0 || device_type >= DEVICE_TYPE_Max)
        return false;

    switch (device_type)
    {
    case DEVICE_TYPE_PCS:
        if(no > PCS_Num_Max)
            return false;
        *start = Node_Addr_PCS + (no - 1) * Node_Num_PCS;
        *end = Node_Addr_PCS + no * Node_Num_PCS - 1;
        return true;
    case DEVICE_TYPE_Air:
        if(no > Air_Num_Max)
            return false;
        *start = Node_Addr_Air + (no - 1) * Node_Num_Air;
        *end = Node_Addr_Air + no * Node_Num_Air - 1;
        return true;
    case DEVICE_TYPE_Meter:
        if(no > Meter_Num_Max)
            return false;
        *start = Node_Addr_Meter + (no - 1) * Node_Num_Meter;
        *end = Node_Addr_Meter + no * Node_Num_Meter - 1;
        return true;
    case DEVICE_TYPE_Fire:
        if(no > Fire_Num_Max)
            return false;
        *start = Node_Addr_Fire + (no - 1) * Node_Num_Fire;
        *end = Node_Addr_Fire + no * Node_Num_Fire - 1;
        return true;
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return false;
        *start = Node_Addr_Doil + (no - 1) * Node_Num_Doil;
        *end = Node_Addr_Doil + no * Node_Num_Doil - 1;
        return true;
    case DEVICE_TYPE_Pv:
        if(no > Pv_Num_Max)
            return false;
        *start = Node_Addr_Pv + (no - 1) * Node_Num_Pv;
        *end = Node_Addr_Pv + no * Node_Num_Pv - 1;
        return true;
    case DEVICE_TYPE_Coolwater:
        if(no > Coolwater_Num_Max)
            return false;
        *start = Node_Addr_Coolwater + (no - 1) * Node_Num_Coolwater;
        *end = Node_Addr_Coolwater + no * Node_Num_Coolwater - 1;
        return true;
    case DEVICE_TYPE_Dehum:
        if(no > Dehum_Num_Max)
            return false;
        *start = Node_Addr_Dehum + (no - 1) * Node_Num_Dehum;
        *end = Node_Addr_Dehum + no * Node_Num_Dehum - 1;
        return true;
    default:
        return false;
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




