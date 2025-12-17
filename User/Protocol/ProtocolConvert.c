#include "ProtocolConvert.h"
#include "usart.h"
#include "cJSON.h"
#include <stdlib.h>
#include "Comm485.h"
#include "ProtocolNode.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
ProtocolConvert_modbus_t ProtocolConvert_modbus[DEVICE_TYPE_Max];
ProtocolConvert_CAN_t ProtocolConvert_CAN[DEVICE_TYPE_Max];

PortConfig_modbus_t PortConfig_modbus[PortConfig_modbus_Num];
PortConfig_CAN_t PortConfig_CAN[PortConfig_CAN_Num];
PortConfig_DI_t PortConfig_DI[PortConfig_DI_Num];
PortConfig_DO_t PortConfig_DO[PortConfig_DO_Num];
PortConfig_NTC_t PortConfig_NTC[PortConfig_NTC_Num];
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
PortConfig_NTC_t* GetPortConfig_NTC(uint8_t no)
{
    if(no == 0 || no > PortConfig_NTC_Num)
        return NULL;

    return &PortConfig_NTC[no - 1];
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
            node[0] = data;
        }
        break;
    case MODEL_TYPE_S16:
        {
            int16_t data = (int16_t)temp;
            node[0] = data;
        }
        break;
    case MODEL_TYPE_U32:
        {
            uint32_t data = (uint32_t)temp;
            node[0] = (data >> 16) & 0xffff;
            node[1] = data & 0xffff;
        }
        break;
    case MODEL_TYPE_S32:
        {
            int32_t data = (int32_t)temp;
            node[0] = (data >> 16) & 0xffff;
            node[1] = data & 0xffff;
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
            uint16_t data = BUILD_U16_AB(byte[0], byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_AB:
        {
            int16_t data = BUILD_S16_AB(byte[0], byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U16_BA:
        {
            uint16_t data = BUILD_U16_BA(byte[0], byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S16_BA:
        {
            int16_t data = BUILD_S16_BA(byte[0], byte[1]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_ABCD:
        {
            uint32_t data = BUILD_U32_ABCD(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_ABCD:
        {
            int32_t data = BUILD_S32_ABCD(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_BADC:
        {
            uint32_t data = BUILD_U32_BADC(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_BADC:
        {
            int32_t data = BUILD_S32_BADC(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_CDAB:
        {
            uint32_t data = BUILD_U32_CDAB(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_CDAB:
        {
            int32_t data = BUILD_S32_CDAB(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_U32_DCBA:
        {
            uint32_t data = BUILD_U32_DCBA(byte[0], byte[1], byte[2], byte[3]);
            temp = (double)data * convert->factor + convert->offset;
        }
        break;
    case DATE_TYPE_S32_DCBA:
        {
            int32_t data = BUILD_S32_DCBA(byte[0], byte[1], byte[2], byte[3]);
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
            uint16_t data = BUILD_U16_AB(byte[0], byte[1]);
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
            uint16_t data = GetBits_16(BUILD_U16_AB(byte[0], byte[1]), convert->bit_field_msb, convert->bit_field_lsb);
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

    DataToNode(node, temp, convert->model_type);
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
    case 1:
        return id + (device_attr->master_addr * 256 + device_attr->slave_addr);
    default:
        return id;
    }
}

uint32_t CAN_ID_Deoffset_calc(uint32_t id, CAN_device_attr_t* device_attr)
{
    switch (device_attr->addr_format)
    {
    case 0:
        return id;
    case 1:
        return id - (device_attr->master_addr * 256 + device_attr->slave_addr);
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

    DataToNode(node, temp, convert->model_type);
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
            PortConfig_modbus_t* modbus = GetPortConfig_modbus((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "p")));
            if(modbus == NULL)
                continue;

            modbus->en         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "e"));
            modbus->baud       = (uint32_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "b"));
            modbus->date_bit   = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "db"));
            modbus->stop_bit   = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "sb"));
            modbus->parity     = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "pa"));
            modbus->device_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array, "dn"));

            cJSON *root_modbus_array_device_attr = cJSON_GetObjectItem(root_modbus_array, "da"); 
            if(modbus->device_num != cJSON_GetArraySize(root_modbus_array_device_attr))
            {
                modbus->device_num = 0;
                continue;
            }
  
            for(uint8_t j = 0; j < modbus->device_num; j++)
            {
                cJSON *root_modbus_array_device_attr_array = cJSON_GetArrayItem(root_modbus_array_device_attr, j);
                modbus->device_attr[j].device_type         = (DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "dt"));
                modbus->device_attr[j].protocol            = (PROTOCOL_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "p"));
                modbus->device_attr[j].device_no           = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "dn"));
                modbus->device_attr[j].device_addr         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_modbus_array_device_attr_array, "da"));
            }
        }
    }

    cJSON *root_CAN = cJSON_GetObjectItem(root, "CAN"); 
    if(root_CAN != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_CAN); i++)
        {
            cJSON *root_CAN_array = cJSON_GetArrayItem(root_CAN, i);
            PortConfig_CAN_t* CAN = GetPortConfig_CAN((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "p")));
            if(CAN == NULL)
                continue;
                
            CAN->en         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "e"));
            CAN->baud       = (uint32_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "b"));
            CAN->device_num = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array, "dn"));

            cJSON *root_CAN_array_device_attr = cJSON_GetObjectItem(root_CAN_array, "da"); 
            if(CAN->device_num != cJSON_GetArraySize(root_CAN_array_device_attr))
            {
                CAN->device_num = 0;
                continue;
            }
                
            for(uint8_t j = 0; j < CAN->device_num; j++)
            {
                cJSON *root_CAN_array_device_attr_array = cJSON_GetArrayItem(root_CAN_array_device_attr, j);
                CAN->device_attr[j].device_type         = (DEVICE_TYPE_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "dt"));
                CAN->device_attr[j].protocol            = (PROTOCOL_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "p"));
                CAN->device_attr[j].device_no           = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "dn"));
                CAN->device_attr[j].master_addr         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "ma"));
                CAN->device_attr[j].slave_addr          = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "sa"));
                CAN->device_attr[j].addr_format         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_CAN_array_device_attr_array, "af"));
            }
        }
    }

    cJSON *root_DI = cJSON_GetObjectItem(root, "DI"); 
    if(root_DI != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_DI); i++)
        {
            cJSON *root_DI_array = cJSON_GetArrayItem(root_DI, i);
            PortConfig_DI_t* DI = GetPortConfig_DI((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "p")));
            if(DI == NULL)
                continue;

            DI->en = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "e"));

            if(DI->en == 0)
                continue;

            DI->signal        = (DI_Signal_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "s"));
            DI->valid         = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "v"));
            DI->trigger_delay = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "td"));
            DI->recover_delay = (uint16_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DI_array, "rd"));
        }
    }

    cJSON *root_DO = cJSON_GetObjectItem(root, "DO"); 
    if(root_DO != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_DO); i++)
        {
            cJSON *root_DO_array = cJSON_GetArrayItem(root_DO, i);
            PortConfig_DO_t* DO = GetPortConfig_DO((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "p")));
            if(DO == NULL)
                continue;

            DO->en = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "e"));

            if(DO->en == 0)
                continue;

            DO->ctrl  = (DO_Ctrl_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "c"));
            DO->valid = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_DO_array, "v"));
        }
    }

    cJSON *root_NTC = cJSON_GetObjectItem(root, "NTC"); 
    if(root_NTC != NULL)
    {
        for(uint8_t i = 0; i < cJSON_GetArraySize(root_NTC); i++)
        {
            cJSON *root_NTC_array = cJSON_GetArrayItem(root_NTC, i);
            PortConfig_NTC_t* NTC = GetPortConfig_NTC((uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_NTC_array, "p")));
            if(NTC == NULL)
                continue;

            NTC->en = (uint8_t)cJSON_GetNumberValue(cJSON_GetObjectItem(root_NTC_array, "e"));
            if(NTC->en == 0)
                continue;

            NTC->temp  = (NTC_Temp_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_NTC_array, "tp"));
            NTC->table = (NTC_Table_e)cJSON_GetNumberValue(cJSON_GetObjectItem(root_NTC_array, "tb"));
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

    if(cJSON_GetNumberValue(root_protocol) == PROTOCOL_MODBUS)//modbus
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
    else if(cJSON_GetNumberValue(root_protocol) == PROTOCOL_CAN)//CAN
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

        printf("modbus port: %d, en: %d, baud: %d, date_bit: %d, stop_bit: %d, parity: %d, device_num: %d\r\n",
            i, modbus->en, modbus->baud, modbus->date_bit, modbus->stop_bit, modbus->parity, modbus->device_num);

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

        printf("CAN port: %d, en: %d, baud: %d, device_num: %d\r\n",
            i, can->en, can->baud, can->device_num);

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

        printf("DI port: %d, en: %d, signal: %d, valid: %d, trigger_delay: %d, recover_delay: %d\r\n",
            i, di->en, di->signal, di->valid, di->trigger_delay, di->recover_delay);

        printf("\r\n");
    }

    for(uint8_t i = 1; i <= PortConfig_DO_Num; i++)
    {
        PortConfig_DO_t* _do = GetPortConfig_DO(i);
        if(_do == NULL)
            return;

        printf("DO port: %d, en: %d, ctrl: %d, valid: %d\r\n",
            i, _do->en, _do->ctrl, _do->valid);

        printf("\r\n");
    }

    for(uint8_t i = 1; i <= PortConfig_NTC_Num; i++)
    {
        PortConfig_NTC_t* ntc = GetPortConfig_NTC(i);
        if(ntc == NULL)
            return;

        printf("NTC port: %d, en: %d, temp: %d, table: %d\r\n",
            i, ntc->en, ntc->temp, ntc->table);

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


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


const char portconfig_json_string[] = "{\"modbus\":[{\"p\":1,\"e\":1,\"b\":9600,\"db\":8,\"sb\":1,\"pa\":0,\"dn\":1,\"da\":[{\"dt\":2,\"p\":0,\"dn\":1,\"da\":1}]},{\"p\":2,\"e\":1,\"b\":9600,\"db\":8,\"sb\":1,\"pa\":0,\"dn\":1,\"da\":[{\"dt\":1,\"p\":0,\"dn\":1,\"da\":1}]}],\"CAN\":[{\"p\":1,\"e\":1,\"b\":250000,\"dn\":1,\"da\":[{\"dt\":0,\"p\":1,\"dn\":1,\"ma\":1,\"sa\":1,\"af\":1}]},{\"p\":2,\"e\":1,\"b\":250000,\"dn\":1,\"da\":[{\"dt\":3,\"p\":2,\"dn\":1,\"ma\":1,\"sa\":1,\"af\":0}]}],\"DI\":[{\"p\":5,\"e\":1,\"s\":4,\"v\":0,\"td\":1000,\"rd\":1000}],\"DO\":[{\"p\":4,\"e\":1,\"c\":3,\"v\":1},{\"p\":5,\"e\":1,\"c\":4,\"v\":1}],\"NTC\":[{\"p\":1,\"e\":1,\"tp\":0,\"tb\":0}]}";



const char protocolconvert_modbus_meter_json_string[] = "{\"protocol\":0,\"device_type\":2,\"area\":{\"area_num\":1,\"area_attr\":[{\"fc\":3,\"a\":97,\"n\":3,\"c\":2000}]},\"node\":{\"node_num\":3,\"node_attr\":[{\"ni\":0,\"mi\":3640,\"mt\":0,\"dt\":1,\"fc\":3,\"a\":97,\"m\":0,\"l\":0,\"f\":1,\"o\":0,\"e\":{\"n\":0}},{\"ni\":1,\"mi\":3641,\"mt\":0,\"dt\":1,\"fc\":3,\"a\":98,\"m\":0,\"l\":0,\"f\":1,\"o\":0,\"e\":{\"n\":0}},{\"ni\":2,\"mi\":3642,\"mt\":0,\"dt\":1,\"fc\":3,\"a\":99,\"m\":0,\"l\":0,\"f\":1,\"o\":0,\"e\":{\"n\":0}}]}}";






void Init_ProtocolConvert(void)
{
    memset(ProtocolConvert_modbus, 0, sizeof(ProtocolConvert_modbus));
    memset(ProtocolConvert_CAN, 0, sizeof(ProtocolConvert_CAN));

    memset(PortConfig_modbus, 0, sizeof(PortConfig_modbus));
    memset(PortConfig_CAN, 0, sizeof(PortConfig_CAN));
    memset(PortConfig_DI, 0, sizeof(PortConfig_DI));
    memset(PortConfig_DO, 0, sizeof(PortConfig_DO));
    memset(PortConfig_NTC, 0, sizeof(PortConfig_NTC));


    char* json = NULL;

    //read
    json = (char*)portconfig_json_string;
    if(json != NULL)
        cJSON_To_PortConfig(json);

    
    //read
    json = (char*)protocolconvert_modbus_meter_json_string;
    if(json != NULL)
        cJSON_To_ProtocolConvert(json);

}

