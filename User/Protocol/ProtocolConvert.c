#include "ProtocolConvert.h"
#include "ModbusRTU.h"
#include "usart.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
Node_PCS_t Node_PCS[PCS_Num_Max];
Node_Air_t Node_Air[Air_Num_Max];
Node_Meter_t Node_Meter[Meter_Num_Max];
Node_FIre_t Node_FIre[FIre_Num_Max];
Node_Doil_t Node_Doil[Doil_Num_Max];
Node_PvPCS_t Node_PvPCS[PvPCS_Num_Max];


ProtocolConvert_485_t ProtocolConvert_485[DEVICE_TYPE_Max];
ProtocolConvert_CAN_t ProtocolConvert_CAN[DEVICE_TYPE_Max];

PortConfig_485_t PortConfig_485[PortConfig_485_Num];
PortConfig_CAN_t PortConfig_CAN[PortConfig_CAN_Num];

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
    case DEVICE_TYPE_FIre:
        if(no > FIre_Num_Max)
            return NULL;
        return &Node_FIre[no - 1].Node[0];
    case DEVICE_TYPE_Doil:
        if(no > Doil_Num_Max)
            return NULL;
        return &Node_Doil[no - 1].Node[0];
    case DEVICE_TYPE_PvPCS:
        if(no > PvPCS_Num_Max)
            return NULL;
        return &Node_PvPCS[no - 1].Node[0];
    default:
        return NULL;
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
ProtocolConvert_485_t* GetProtocolConvert_485(DEVICE_TYPE_e device_type)
{
    if(device_type >= DEVICE_TYPE_Max)
        return NULL;

    return &ProtocolConvert_485[device_type];
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
PortConfig_485_t* GetPortConfig_485(uint8_t no)
{
    if(no == 0 || no > PortConfig_485_Num)
        return NULL;

    return &PortConfig_485[no - 1];
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
void ConvertToNode_485(uint16_t* node, uint8_t* byte, _485_node_attr_t* convert)
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
void Comm_485_Pro(uint8_t port, uint8_t *tx_buff, uint8_t *rx_buff)
{
    PortConfig_485_t* _485 = GetPortConfig_485(port);
    if(_485 == NULL)
        return;

    for(uint8_t device_num = 0; device_num < _485->device_num; device_num++)//扫描所有设备
    {
        //获取协议
        ProtocolConvert_485_t* convert = GetProtocolConvert_485(_485->device_attr[device_num].device_type);
        if(convert == NULL)
            continue;
        
        for(uint8_t area_num = 0; area_num < convert->area_num; area_num++)//扫描所有区域
        {
            if(++_485->device_attr[device_num].cyclecnt[area_num] >= convert->area_attr[area_num].cycle)//定时查询
            {
                _485->device_attr[device_num].cyclecnt[area_num] = 0;

                uint16_t tx_len = 0;
                //encode
                ModbusRTU_BuildReadHolding(
                    _485->device_attr[device_num].device_addr,//设备地址
                    convert->area_attr[area_num].reg_addr,//寄存器地址
                    convert->area_attr[area_num].reg_num,//寄存器数量
                    tx_buff,
                    &tx_len
                );

                uint16_t rx_len = _485_Tx_And_Rx(port, tx_buff, tx_len, rx_buff, Uart_Rx_Buff_Size);

                const uint8_t* data;
                uint16_t data_len;

                //校验
                int32_t result = ModbusRTU_ParseReadHoldingRsp(
                        rx_buff,
                        rx_len,//接收到的数据长度
                        _485->device_attr[device_num].device_addr,//设备地址
                        convert->area_attr[area_num].reg_num,//寄存器数量
                        &data,//存寄存器数据的指针
                        &data_len//字节数
                    );

                if(result == MB_OK)
                {
                    uint16_t reg_addr = convert->area_attr[area_num].reg_addr;

                    for(uint16_t reg_num = 0; reg_num < convert->area_attr[area_num].reg_num; reg_num++)//扫描读取出来的寄存器
                    {
                        for(uint16_t node_num = 0; node_num < convert->node_num; node_num++)//扫描所有点表
                        {
                            if(convert->node_attr[node_num].reg_addr == reg_addr)//匹配点表
                            {
                                ConvertToNode_485(GetNode(_485->device_attr[device_num].device_type, _485->device_attr[device_num].device_no), 
                                                (uint8_t*)data,
                                                &convert->node_attr[node_num]);

                                break;
                            }
                        }
                        data += 2;//指针增加2字节
                        reg_addr++;//寄存器地址增加1
                    } 
                }
            }
        }
    }
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



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


