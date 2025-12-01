#include "ProtocolConvert.h"
#include "ModbusRTU.h"
#include "usart.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define Node_PCS_Max 2
#define Node_Air_Max 2
#define Node_Meter_Max 2
#define Node_FIre_Max 2
#define Node_Doil_Max 2
#define Node_PvPCS_Max 2

typedef struct
{
    uint16_t Node[512];
}Node_PCS_t;
Node_PCS_t Node_PCS[Node_PCS_Max];

typedef struct
{
    uint16_t Node[512];
}Node_Air_t;
Node_Air_t Node_Air[Node_Air_Max];

typedef struct
{
    uint16_t Node[512];
}Node_Meter_t;
Node_Meter_t Node_Meter[Node_Meter_Max];

typedef struct
{
    uint16_t Node[512];
}Node_FIre_t;
Node_FIre_t Node_FIre[Node_FIre_Max];

typedef struct
{
    uint16_t Node[512];
}Node_Doil_t;
Node_Doil_t Node_Doil[Node_Doil_Max];

typedef struct
{
    uint16_t Node[512];
}Node_PvPCS_t;
Node_PvPCS_t Node_PvPCS[Node_PvPCS_Max];



PortConfig_t PortConfig;






/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint16_t* GetNode(ProtocolConvert_device_type_t device_type, uint8_t no)
{
    switch (device_type)
    {
    // case DEVICE_TYPE_PCS:
    //     return &Node_PCS[no][0];
    // case DEVICE_TYPE_Air:
    //     return &Node_Air[no][0];
    // case DEVICE_TYPE_Meter:
    //     return &Node_Meter[no][0];
    // case DEVICE_TYPE_FIre:
    //     return &Node_FIre[no][0];
    // case DEVICE_TYPE_Doil:
    //     return &Node_Doil[no][0];
    // case DEVICE_TYPE_PvPCS:
    //     return &Node_PvPCS[no][0];
    default:
        return NULL;
    }
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
void DataToNode(uint16_t* node, double temp, ProtocolConvert_model_type_t model_type, uint16_t id)
{
    switch (model_type)
    {
    case MODEL_TYPE_U16:
        {
            uint16_t data = (uint16_t)temp;

            node[id] &= 0;
            node[id] |= data;
        }
        break;
    case MODEL_TYPE_S16:
        {
            int16_t data = (int16_t)temp;

            node[id] &= 0;
            node[id] |= data;
        }
        break;
    case MODEL_TYPE_U32:
        {
            uint32_t data = (uint32_t)temp;

            node[id] &= 0;
            node[id] |= (data >> 16) & 0xffff;

            node[id] &= 0;
            node[id] |= data & 0xffff;
        }
        break;
    case MODEL_TYPE_S32:
        {
            int32_t data = (int32_t)temp;

            node[id] &= 0;
            node[id] |= (data >> 16) & 0xffff;

            node[id] &= 0;
            node[id] |= data & 0xffff;
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
void ProtocolConvert_485(uint16_t* node, uint8_t* byte, ProtocolConvert_485_t* convert)
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

    DataToNode(node, temp, convert->model_type, convert->model_id);
}


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void ProtocolConvert_CAN(uint16_t* node, uint8_t* byte, ProtocolConvert_CAN_t* convert)
{
    double temp = 0.0;

    switch (convert->date_type)
    {
    case DATE_TYPE_BITS:
        {
            uint32_t data = GetBits_8(byte[convert->frame_byte], convert->bit_field_msb, convert->bit_field_lsb);
            temp = (double)data * convert->factor + convert->offset;
        }
        /* code */
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

    DataToNode(node, temp, convert->model_type, convert->model_id);
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm_485_Pro(uint8_t port)
{
    PortConfig_485_t *config = &PortConfig._485[port];

    for(int i = 0; i < config->device_num; i++)//扫描所有设备
    {
        for(int j = 0; j < config->device_attr[i].area_num; j++)//扫描所有区域
        {
            config->device_attr[i].area_attr[j].cyclecnt++;
            if(config->device_attr[i].area_attr[j].cyclecnt >= config->device_attr[i].area_attr[j].cycle)//定时查询
            {
                config->device_attr[i].area_attr[j].cyclecnt = 0;

                //send
                uint16_t tx_len = 0;
                uint16_t rx_len = 0;
                uint16_t quantity = config->device_attr[i].area_attr[j].end_addr - config->device_attr[i].area_attr[j].start_addr + 1;
                //encode
                ModbusRTU_BuildReadHolding(
                    config->device_attr[i].device_addr,//设备地址
                    config->device_attr[i].area_attr[j].start_addr,//寄存器地址
                    quantity,//寄存器数量
                    config->tx_buff,
                    &tx_len
                );

                //send and receive
                rx_len = _485_Tx_And_Rx(port, config->tx_buff, tx_len, config->rx_buff, Uart_Rx_Buff_Size);

                const uint8_t* data;
                uint16_t data_len;

                //校验
                int32_t result = ModbusRTU_ParseReadHoldingRsp(
                        config->rx_buff,
                        rx_len,//接收到的数据长度
                        config->device_attr[i].device_addr,//从机地址
                        quantity,//寄存器数量
                        &data,//存寄存器数据的指针
                        &data_len//寄存器数据长度
                    );

                if(result == MB_OK)
                {

                    // for(int k = 0; k < quantity; k++)
                    // {
                    //     uint16_t node = config->device_attr[i].area_attr[j].start_addr + k;
                    //     ProtocolConvert_485(&node, &data[k * 2], &config->device_attr[i].area_attr[j]);
                    // }




                    // ProtocolConvert_485(GetNode(config->device_attr[i].device_type, config->device_attr[i].device_no), 
                    // &data[k * 2], 
                    // &config->device_attr[i].area_attr[j]);




                }
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


