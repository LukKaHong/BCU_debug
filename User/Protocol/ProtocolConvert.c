#include "ProtocolConvert.h"
#include "usart.h"
#include "cJSON.h"
#include <stdlib.h>
#include "Comm485.h"

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


ProtocolConvert_modbus_t ProtocolConvert_modbus[DEVICE_TYPE_Max];
ProtocolConvert_CAN_t ProtocolConvert_CAN[DEVICE_TYPE_Max];

PortConfig_modbus_t PortConfig_modbus[PortConfig_modbus_Num];
PortConfig_CAN_t PortConfig_CAN[PortConfig_CAN_Num];
PortConfig_DI_t PortConfig_DI[PortConfig_DI_Num];
PortConfig_DO_t PortConfig_DO[PortConfig_DO_Num];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Printf_PortConfig(void);
static void Printf_ProtocolConvert_modbus(void);
static void Printf_ProtocolConvert_CAN(void);

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
ProtocolConvert_modbus_t* GetProtocolConvert_modbus(DEVICE_TYPE_e device_type)
{
    if(device_type >= DEVICE_TYPE_Max)
        return NULL;

    return &ProtocolConvert_modbus[device_type];
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
ProtocolConvert_CAN_t* GetProtocolConvert_CAN(DEVICE_TYPE_e device_type)
{
    if(device_type >= DEVICE_TYPE_Max)
        return NULL;

    return &ProtocolConvert_CAN[device_type];
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
PortConfig_modbus_t* GetPortConfig_modbus(uint8_t no)
{
    if(no == 0 || no > PortConfig_modbus_Num)
        return NULL;

    return &PortConfig_modbus[no - 1];
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
PortConfig_CAN_t* GetPortConfig_CAN(uint8_t no)
{
    if(no == 0 || no > PortConfig_CAN_Num)
        return NULL;

    return &PortConfig_CAN[no - 1];
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
PortConfig_DI_t* GetPortConfig_DI(uint8_t no)
{
    if(no == 0 || no > PortConfig_DI_Num)
        return NULL;

    return &PortConfig_DI[no - 1];
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
PortConfig_DO_t* GetPortConfig_DO(uint8_t no)
{
    if(no == 0 || no > PortConfig_DO_Num)
        return NULL;

    return &PortConfig_DO[no - 1];
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint16_t GetBits_16(uint16_t data, uint8_t bit_field_msb, uint8_t bit_field_lsb)
{
    data <<= 15 - bit_field_msb;
    data >>= 15 - bit_field_msb + bit_field_lsb;
    return data;
}


uint8_t GetBits_8(uint8_t data, uint8_t bit_field_msb, uint8_t bit_field_lsb)
{
    data <<= 7 - bit_field_msb;
    data >>= 7 - bit_field_msb + bit_field_lsb;
    return data;
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void DataToNode(uint16_t* node, double temp, MODEL_TYPE_e model_type)
{
    switch (model_type)
    {
    case MODEL_TYPE_U16:
        {
            uint16_t data = (uint16_t)temp;

            node[0] &= 0;
            node[0] |= data;
        }
        break;
    case MODEL_TYPE_S16:
        {
            int16_t data = (int16_t)temp;

            node[0] &= 0;
            node[0] |= data;
        }
        break;
    case MODEL_TYPE_U32:
        {
            uint32_t data = (uint32_t)temp;

            node[0] &= 0;
            node[0] |= (data >> 16) & 0xffff;

            node[1] &= 0;
            node[1] |= data & 0xffff;
        }
        break;
    case MODEL_TYPE_S32:
        {
            int32_t data = (int32_t)temp;

            node[0] &= 0;
            node[0] |= (data >> 16) & 0xffff;

            node[1] &= 0;
            node[1] |= data & 0xffff;
        }
        break;
    default:
        break;
    }
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void ConvertToNode_modbus(uint16_t* node, uint8_t* byte, modbus_node_attr_t* convert)
{
    double temp = 0.0;

    switch (convert->date_type)
    {
    case DATE_TYPE_BITS:
        {
            uint32_t data = GetBits_8(byte[0], convert->bit_field_msb, convert->bit_field_lsb);
            temp = (double)data * convert->factor + convert->offset;
        }
        /* code */
        break;
    case DATE_TYPE_U16_AB:
        {
            uint16_t data = (uint16_t)((byte[0] << 8) | byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_AB:
        {
            int16_t data = (int16_t)((byte[0] << 8) | byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U16_BA:
        {
            uint16_t data = (uint16_t)((byte[1] << 8) | byte[0]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_BA:
        {
            int16_t data = (int16_t)((byte[1] << 8) | byte[0]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_ABCD:
        {
            uint32_t data = (uint32_t)((byte[0] << 24) | (byte[1] << 16) | (byte[2] << 8) | byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_ABCD:
        {
            int32_t data = (int32_t)((byte[0] << 24) | (byte[1] << 16) | (byte[2] << 8) | byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_BADC:
        {
            uint32_t data = (uint32_t)((byte[1] << 24) | (byte[0] << 16) | (byte[3] << 8) | byte[2]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_BADC:
        {
            int32_t data = (int32_t)((byte[1] << 24) | (byte[0] << 16) | (byte[3] << 8) | byte[2]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_CDAB:
        {
            uint32_t data = (uint32_t)((byte[2] << 24) | (byte[3] << 16) | (byte[0] << 8) | byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_CDAB:
        {
            int32_t data = (int32_t)((byte[2] << 24) | (byte[3] << 16) | (byte[0] << 8) | byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_DCBA:
        {
            uint32_t data = (uint32_t)((byte[3] << 24) | (byte[2] << 16) | (byte[1] << 8) | byte[0]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_DCBA:
        {
            int32_t data = (int32_t)((byte[3] << 24) | (byte[2] << 16) | (byte[1] << 8) | byte[0]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_F32_ABCD:
        break;
    case DATE_TYPE_F32_BADC:
        break;
    case DATE_TYPE_F32_CDAB:
        break;
    case DATE_TYPE_F32_DCBA:
        break;
    case DATE_TYPE_Enum:
        {
            uint16_t data = (uint16_t)((byte[0] << 8) | byte[1]);
            for (uint16_t i = 0; i < convert->enum_num; i++)
            {
                if (data == convert->enum_convert[i].value_src)
                {
                    temp = (double)convert->enum_convert[i].value_dst * convert->factor + convert->offset;
                    break;
                }
            }
        }
        /* code */
        break;
    case DATE_TYPE_BITS_Enum:
        {
            uint16_t data = GetBits_16((uint16_t)((byte[0] << 8) | byte[1]), convert->bit_field_msb, convert->bit_field_lsb);
            for (uint16_t i = 0; i < convert->enum_num; i++)
            {
                if (data == convert->enum_convert[i].value_src)
                {
                    temp = (double)convert->enum_convert[i].value_dst * convert->factor + convert->offset;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    DataToNode(&node[convert->model_id], temp, convert->model_type);
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint32_t CAN_ID_offset_calc(uint32_t id, CAN_device_attr_t* device_attr)
{
    switch (device_attr->addr_format)
    {
    case 0:
        return id;
    default:
        return id;
    }
}

void ConvertToNode_CAN(uint16_t* node, uint8_t* byte, CAN_node_attr_t* convert)
{
    double temp = 0.0;

    switch (convert->date_type)
    {
    case DATE_TYPE_BITS:
        {
            uint32_t data = GetBits_8(byte[convert->frame_byte], convert->bit_field_msb, convert->bit_field_lsb);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U16_AB:
        {
            uint16_t data = (uint16_t)((byte[convert->frame_byte] << 8) | byte[convert->frame_byte + 1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_AB:
        {
            int16_t data = (int16_t)((byte[convert->frame_byte] << 8) | byte[convert->frame_byte + 1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U16_BA:
        {
            uint16_t data = (uint16_t)((byte[convert->frame_byte + 1] << 8) | byte[convert->frame_byte]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_BA:
        {
            int16_t data = (int16_t)((byte[convert->frame_byte + 1] << 8) | byte[convert->frame_byte]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_ABCD:
        {
            uint32_t data = (uint32_t)((byte[convert->frame_byte] << 24) | (byte[convert->frame_byte + 1] << 16) | (byte[convert->frame_byte + 2] << 8) | byte[convert->frame_byte + 3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_ABCD:
        {
            int32_t data = (int32_t)((byte[convert->frame_byte] << 24) | (byte[convert->frame_byte + 1] << 16) | (byte[convert->frame_byte + 2] << 8) | byte[convert->frame_byte + 3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_BADC:
        {
            uint32_t data = (uint32_t)((byte[convert->frame_byte + 1] << 24) | (byte[convert->frame_byte] << 16) | (byte[convert->frame_byte + 3] << 8) | byte[convert->frame_byte + 2]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_BADC:
        {
            int32_t data = (int32_t)((byte[convert->frame_byte + 1] << 24) | (byte[convert->frame_byte] << 16) | (byte[convert->frame_byte + 3] << 8) | byte[convert->frame_byte + 2]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_CDAB:
        {
            uint32_t data = (uint32_t)((byte[convert->frame_byte + 2] << 24) | (byte[convert->frame_byte + 3] << 16) | (byte[convert->frame_byte] << 8) | byte[convert->frame_byte + 1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_CDAB:
        {
            int32_t data = (int32_t)((byte[convert->frame_byte + 2] << 24) | (byte[convert->frame_byte + 3] << 16) | (byte[convert->frame_byte] << 8) | byte[convert->frame_byte + 1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_DCBA:
        {
            uint32_t data = (uint32_t)((byte[convert->frame_byte + 3] << 24) | (byte[convert->frame_byte + 2] << 16) | (byte[convert->frame_byte + 1] << 8) | byte[convert->frame_byte]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_DCBA:
        {
            int32_t data = (int32_t)((byte[convert->frame_byte + 3] << 24) | (byte[convert->frame_byte + 2] << 16) | (byte[convert->frame_byte + 1] << 8) | byte[convert->frame_byte]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_F32_ABCD:
        break;
    case DATE_TYPE_F32_BADC:
        break;
    case DATE_TYPE_F32_CDAB:
        break;
    case DATE_TYPE_F32_DCBA:
        break;
    case DATE_TYPE_Enum:
        {
            uint16_t data = (uint16_t)(byte[convert->frame_byte]);
            for (uint16_t i = 0; i < convert->enum_num; i++)
            {
                if (data == convert->enum_convert[i].value_src)
                {
                    temp = (double)convert->enum_convert[i].value_dst * convert->factor + convert->offset;
                    break;
                }
            }
        }
        /* code */
        break;
    case DATE_TYPE_BITS_Enum:
        {
            uint16_t data = (uint16_t)GetBits_8(byte[convert->frame_byte], convert->bit_field_msb, convert->bit_field_lsb);
            for (uint16_t i = 0; i < convert->enum_num; i++)
            {
                if (data == convert->enum_convert[i].value_src)
                {
                    temp = (double)convert->enum_convert[i].value_dst * convert->factor + convert->offset;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    DataToNode(&node[convert->model_id], temp, convert->model_type);
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm_CAN_Pro(uint8_t port, CanMsgType *msg)
{
    PortConfig_CAN_t* CAN = GetPortConfig_CAN(port);
    if(CAN == NULL)
        return;

    for(uint8_t device_num = 0; device_num < CAN->device_num; device_num++)//扫描所有设备
    {
        //获取协议
        ProtocolConvert_CAN_t* convert = GetProtocolConvert_CAN(CAN->device_attr[device_num].device_type);
        if(convert == NULL)
            continue;

        for(uint16_t node_num = 0; node_num < convert->node_num; node_num++)//扫描所有点表
        {
            if(convert->node_attr[node_num].frame_ID == CAN_ID_offset_calc(msg->id, &CAN->device_attr[device_num]))//匹配ID
            {
                ConvertToNode_CAN(GetNode(CAN->device_attr[device_num].device_type, CAN->device_attr[device_num].device_no), 
                                                msg->data,
                                                &convert->node_attr[node_num]);

                break;
            }
        }
    }
}



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void cJSON_To_PortConfig(char *message)
{
    printf("message : \n%s\n", message);

    cJSON *root = cJSON_Parse(message);
    if(root == NULL)
    {
        const char *ep = cJSON_GetErrorPtr();
        if (ep != NULL)
        {
            printf("cJSON parse error near: %s\r\n", ep);
        }
        return;
    }

    cJSON *root_modbus = cJSON_GetObjectItem(root, "modbus"); 
    if(root_modbus != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_modbus); i++)
        {
            cJSON *root_modbus_array = cJSON_GetArrayItem(root_modbus, i);
            PortConfig_modbus_t* modbus = GetPortConfig_modbus((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "port")));
            if(modbus == NULL)
                continue;
            modbus->baud       = (uint32_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "baud"));
            modbus->date_bit   = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "date_bit"));
            modbus->stop_bit   = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "stop_bit"));
            modbus->parity     = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "parity"));
            modbus->device_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "device_num"));

            cJSON *root_modbus_array_device_attr = cJSON_GetObjectItem(root_modbus_array, "device_attr"); 
            if(modbus->device_num != cJSON_GetArraySize(root_modbus_array_device_attr))
            {
                modbus->device_num = 0;
                continue;
            }
  
            for(uint8_t j = 0; j < modbus->device_num; j++)
            {
                cJSON *root_modbus_array_device_attr_array = cJSON_GetArrayItem(root_modbus_array_device_attr, j);
                modbus->device_attr[j].device_type         = (DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "device_type"));
                modbus->device_attr[j].device_no           = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "device_no"));
                modbus->device_attr[j].device_addr         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "device_addr"));
            }
        }
    }

    cJSON *root_CAN = cJSON_GetObjectItem(root, "CAN"); 
    if(root_CAN != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_CAN); i++)
        {
            cJSON *root_CAN_array = cJSON_GetArrayItem(root_CAN, i);
            PortConfig_CAN_t* CAN = GetPortConfig_CAN((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "port")));
            if(CAN == NULL)
                continue;
            CAN->baud       = (uint32_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "baud"));
            CAN->device_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "device_num"));

            cJSON *root_CAN_array_device_attr = cJSON_GetObjectItem(root_CAN_array, "device_attr"); 
            if(CAN->device_num != cJSON_GetArraySize(root_CAN_array_device_attr))
            {
                CAN->device_num = 0;
                continue;
            }
                
            for(uint8_t j = 0; j < CAN->device_num; j++)
            {
                cJSON *root_CAN_array_device_attr_array = cJSON_GetArrayItem(root_CAN_array_device_attr, j);
                CAN->device_attr[j].device_type         = (DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "device_type"));
                CAN->device_attr[j].device_no           = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "device_no"));
                CAN->device_attr[j].master_addr         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "master_addr"));
                CAN->device_attr[j].slave_addr          = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "slave_addr"));
                CAN->device_attr[j].addr_format         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "addr_format"));
            }
        }
    }

    cJSON *root_DI = cJSON_GetObjectItem(root, "DI"); 
    if(root_DI != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_DI); i++)
        {
            cJSON *root_DI_array = cJSON_GetArrayItem(root_DI, i);
            PortConfig_DI_t* DI = GetPortConfig_DI((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "port")));
            if(DI == NULL)
                continue;

            DI->en = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "en"));

            if(DI->en == 0)
                continue;

            DI->signal        = (DI_Signal_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "signal"));
            DI->valid         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "valid"));
            DI->trigger_delay = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "trigger_delay"));
            DI->recover_delay = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "recover_delay"));
        }
    }

    cJSON *root_DO = cJSON_GetObjectItem(root, "DO"); 
    if(root_DO != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_DO); i++)
        {
            cJSON *root_DO_array = cJSON_GetArrayItem(root_DO, i);
            PortConfig_DO_t* DO = GetPortConfig_DO((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "port")));
            if(DO == NULL)
                continue;

            DO->en = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "en"));

            if(DO->en == 0)
                continue;

            DO->signal        = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "signal"));
            DO->valid         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "valid"));
        }
    }

    Printf_PortConfig();
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void cJSON_To_ProtocolConvert(char *message)
{
    printf("message : %s\n", message);

    cJSON *root = cJSON_Parse(message);
    if(root == NULL)
    {
        const char *ep = cJSON_GetErrorPtr();
        if (ep != NULL)
        {
            printf("cJSON parse error near: %s\r\n", ep);
        }
        return;
    }

    cJSON *root_protocol = cJSON_GetObjectItem(root, "protocol");

    if(cJSON_GetNumberValue(root_protocol) == 0)//modbus
    {
        ProtocolConvert_modbus_t* convert = GetProtocolConvert_modbus((DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "device_type")));

        cJSON *root_area = cJSON_GetObjectItem(root, "area");
        convert->area_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_area, "area_num"));

        cJSON *root_area_area_attr = cJSON_GetObjectItem(root_area, "area_attr");
        if(convert->area_num != cJSON_GetArraySize(root_area_area_attr))
        {
            convert->area_num = 0;
            return;
        }

        for(uint8_t i = 0; i < convert->area_num; i++)
        {
            cJSON *root_area_area_attr_array = cJSON_GetArrayItem(root_area_area_attr, i);
            convert->area_attr[i].fun_code   = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_area_area_attr_array, "fc"));
            convert->area_attr[i].reg_addr   = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_area_area_attr_array, "a"));
            convert->area_attr[i].reg_num    = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_area_area_attr_array, "n"));
            convert->area_attr[i].cycle      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_area_area_attr_array, "c"));
        }
        
        cJSON *root_node = cJSON_GetObjectItem(root, "node");
        convert->node_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node, "node_num"));

        cJSON *root_node_node_attr = cJSON_GetObjectItem(root_node, "node_attr");
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_node_node_attr); i++)
        {
            cJSON *root_node_node_attr_array    = cJSON_GetArrayItem(root_node_node_attr, i);
            uint8_t node_index = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "ni"));
            convert->node_attr[node_index].model_id      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "mi"));
            convert->node_attr[node_index].model_type    = (MODEL_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "mt"));
            convert->node_attr[node_index].date_type     = (DATE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "dt"));
            convert->node_attr[node_index].fun_code      = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "fc"));
            convert->node_attr[node_index].reg_addr      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "a"));
            convert->node_attr[node_index].bit_field_msb = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "m"));
            convert->node_attr[node_index].bit_field_lsb = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "l"));
            convert->node_attr[node_index].factor        = (float)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "f"));
            convert->node_attr[node_index].offset        = (float)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "o"));

            cJSON *root_node_node_attr_enum = cJSON_GetObjectItem(root_node_node_attr_array, "e");
            convert->node_attr[node_index].enum_num      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_enum, "n"));

            cJSON *root_node_node_attr_enum_enum_attr = cJSON_GetObjectItem(root_node_node_attr_enum, "a");
            if(convert->node_attr[node_index].enum_num != cJSON_GetArraySize(root_node_node_attr_enum_enum_attr))
            {
                convert->node_attr[node_index].enum_num = 0;
                continue;
            }

            for(uint16_t j = 0; j < convert->node_attr[node_index].enum_num; j++)
            {
                cJSON *root_node_node_attr_enum_enum_attr_array = cJSON_GetArrayItem(root_node_node_attr_enum_enum_attr, j);

                convert->node_attr[node_index].enum_convert[j].value_src = (uint16_t)atoi(root_node_node_attr_enum_enum_attr_array->string);
                convert->node_attr[node_index].enum_convert[j].value_dst = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_enum_enum_attr, root_node_node_attr_enum_enum_attr_array->string));
            }
        }

        Printf_ProtocolConvert_modbus();
    }
    else if(cJSON_GetNumberValue(root_protocol) == 1)//CAN
    {
        ProtocolConvert_CAN_t* convert = GetProtocolConvert_CAN((DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "device_type")));

        cJSON *root_node = cJSON_GetObjectItem(root, "node");
        convert->node_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node, "node_num"));

        cJSON *root_node_node_attr = cJSON_GetObjectItem(root_node, "node_attr");
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_node_node_attr); i++)
        {
            cJSON *root_node_node_attr_array    = cJSON_GetArrayItem(root_node_node_attr, i);
            uint8_t node_index = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "ni"));
            convert->node_attr[node_index].model_id      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "mi"));
            convert->node_attr[node_index].model_type    = (MODEL_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "mt"));
            convert->node_attr[node_index].date_type     = (DATE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "dt"));
            convert->node_attr[node_index].frame_ID      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "fi"));
            convert->node_attr[node_index].frame_byte    = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "fb"));
            convert->node_attr[node_index].bit_field_msb = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "m"));
            convert->node_attr[node_index].bit_field_lsb = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "l"));
            convert->node_attr[node_index].factor        = (float)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "f"));
            convert->node_attr[node_index].offset        = (float)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_array, "o"));

            cJSON *root_node_node_attr_enum = cJSON_GetObjectItem(root_node_node_attr_array, "e");
            convert->node_attr[node_index].enum_num      = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_enum, "n"));

            cJSON *root_node_node_attr_enum_enum_attr = cJSON_GetObjectItem(root_node_node_attr_enum, "a");
            if(convert->node_attr[node_index].enum_num != cJSON_GetArraySize(root_node_node_attr_enum_enum_attr))
            {
                convert->node_attr[node_index].enum_num = 0;
                continue;
            }

            for(uint16_t j = 0; j < convert->node_attr[node_index].enum_num; j++)
            {
                cJSON *root_node_node_attr_enum_enum_attr_array = cJSON_GetArrayItem(root_node_node_attr_enum_enum_attr, j);

                convert->node_attr[node_index].enum_convert[j].value_src = (uint16_t)atoi(root_node_node_attr_enum_enum_attr_array->string);
                convert->node_attr[node_index].enum_convert[j].value_dst = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_node_node_attr_enum_enum_attr, root_node_node_attr_enum_enum_attr_array->string));
            }
        }

        Printf_ProtocolConvert_CAN();
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Printf_PortConfig(void)
{
    for(uint8_t i = 1; i <= PortConfig_modbus_Num; i++)
    {
        PortConfig_modbus_t* modbus = GetPortConfig_modbus(i);
        if(modbus == NULL)
            return;

        printf("modbus port: %d, baud: %d, date_bit: %d, stop_bit: %d, parity: %d, device_num: %d\r\n",
            i, modbus->baud, modbus->date_bit, modbus->stop_bit, modbus->parity, modbus->device_num);

        for(uint8_t j = 0; j < modbus->device_num; j++)
        {
            printf("----- %d : modbus device_type: %d, device_no: %d, device_addr: %d\r\n",
                j, modbus->device_attr[j].device_type, modbus->device_attr[j].device_no, modbus->device_attr[j].device_addr);
        }
        printf("\r\n");
    }

    for(uint8_t i = 1; i <= PortConfig_CAN_Num; i++)
    {
        PortConfig_CAN_t* can = GetPortConfig_CAN(i);
        if(can == NULL)
            return;

        printf("CAN port: %d, baud: %d, device_num: %d\r\n",
            i, can->baud, can->device_num);

        for(uint8_t j = 0; j < can->device_num; j++)
        {
            printf("----- %d : CAN device_type: %d, device_no: %d, master_addr: %d, slave_addr: %d, addr_format: %d\r\n",
                j, can->device_attr[j].device_type, can->device_attr[j].device_no, can->device_attr[j].master_addr, can->device_attr[j].slave_addr, can->device_attr[j].addr_format);
        }
        printf("\r\n");
    }

    for(uint8_t i = 1; i <= PortConfig_DI_Num; i++)
    {
        PortConfig_DI_t* di = GetPortConfig_DI(i);
        if(di == NULL)
            return;

        printf("DI port: %d, signal: %d, valid: %d, trigger_delay: %d, recover_delay: %d\r\n",
            i, di->signal, di->valid, di->trigger_delay, di->recover_delay);

        printf("\r\n");
    }

    for(uint8_t i = 1; i <= PortConfig_DO_Num; i++)
    {
        PortConfig_DO_t* _do = GetPortConfig_DO(i);
        if(_do == NULL)
            return;

        printf("DO port: %d, signal: %d, valid: %d\r\n",
            i, _do->signal, _do->valid);

        printf("\r\n");
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Printf_ProtocolConvert_modbus(void)
{
    for(DEVICE_TYPE_e i = DEVICE_TYPE_PCS; i < DEVICE_TYPE_Max; i++)
    {
        ProtocolConvert_modbus_t* convert = GetProtocolConvert_modbus(i);

        printf("modbus device_type: %d, area_num: %d, node_num: %d\r\n",
            i, convert->area_num, convert->node_num);

        for(uint8_t j = 0; j < convert->area_num; j++)
        {
            printf("----- area %d : fun_code: %d, reg_addr: %d, reg_num: %d, cycle: %d\r\n",
                j, convert->area_attr[j].fun_code, convert->area_attr[j].reg_addr, convert->area_attr[j].reg_num, convert->area_attr[j].cycle);
        }

        for(uint8_t j = 0; j < convert->node_num; j++)
        {
            printf("----- node %d : model_id: %d, model_type: %d, date_type: %d, fun_code: %d, reg_addr: %d, bit_field_msb: %d, bit_field_lsb: %d, factor: %f, offset: %f, enum_num: %d\r\n",
                j, convert->node_attr[j].model_id, convert->node_attr[j].model_type, convert->node_attr[j].date_type, convert->node_attr[j].fun_code, convert->node_attr[j].reg_addr, convert->node_attr[j].bit_field_msb, convert->node_attr[j].bit_field_lsb, convert->node_attr[j].factor, convert->node_attr[j].offset, convert->node_attr[j].enum_num);
        }

        printf("\r\n");
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Printf_ProtocolConvert_CAN(void)
{
    for(DEVICE_TYPE_e i = DEVICE_TYPE_PCS; i < DEVICE_TYPE_Max; i++)
    {
        ProtocolConvert_CAN_t* convert = GetProtocolConvert_CAN(i);

        printf("CAN device_type: %d, node_num: %d\r\n",
            i, convert->node_num);

        for(uint8_t j = 0; j < convert->node_num; j++)
        {
            printf("----- node %d : model_id: %d, model_type: %d, date_type: %d, frame_ID: %d, frame_byte: %d,bit_field_msb: %d, bit_field_lsb: %d, factor: %f, offset: %f, enum_num: %d\r\n",
                j, convert->node_attr[j].model_id, convert->node_attr[j].model_type, convert->node_attr[j].date_type, convert->node_attr[j].frame_ID, convert->node_attr[j].frame_byte, convert->node_attr[j].bit_field_msb, convert->node_attr[j].bit_field_lsb, convert->node_attr[j].factor, convert->node_attr[j].offset, convert->node_attr[j].enum_num);
        }
        
        printf("\r\n");
    }
}

