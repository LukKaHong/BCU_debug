#ifndef __PROTOCOL_CONVERT_H__
#define __PROTOCOL_CONVERT_H__

#include "main.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef enum
{
    DEVICE_TYPE_PCS    = 0,
    DEVICE_TYPE_Air    = 1,
    DEVICE_TYPE_Meter  = 2,
    DEVICE_TYPE_FIre   = 3,
    DEVICE_TYPE_Doil   = 4,
    DEVICE_TYPE_PvPCS  = 5,
}ProtocolConvert_device_type_t;



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef enum
{
    MODEL_TYPE_U16    = 0,
    MODEL_TYPE_S16    = 1,
    MODEL_TYPE_U32    = 2,
    MODEL_TYPE_S32    = 3,
}ProtocolConvert_model_type_t;



typedef enum
{
    DATE_TYPE_BITS      = 0,
    DATE_TYPE_U16_AB    = 1,
    DATE_TYPE_S16_AB    = 2,
    DATE_TYPE_U16_BA    = 3,
    DATE_TYPE_S16_BA    = 4,
    DATE_TYPE_U32_ABCD  = 5,
    DATE_TYPE_S32_ABCD  = 6,
    DATE_TYPE_U32_BADC  = 7,
    DATE_TYPE_S32_BADC  = 8,
    DATE_TYPE_U32_CDAB  = 9,
    DATE_TYPE_S32_CDAB  = 10,
    DATE_TYPE_U32_DCBA  = 11,
    DATE_TYPE_S32_DCBA  = 12,
    DATE_TYPE_F32_ABCD  = 13,
    DATE_TYPE_F32_BADC  = 14,
    DATE_TYPE_F32_CDAB  = 15,
    DATE_TYPE_F32_DCBA  = 16,
    DATE_TYPE_Enum      = 17,
    DATE_TYPE_BITS_Enum = 18,
}ProtocolConvert_date_type_t;


typedef struct
{
    uint16_t value_src;
    uint16_t value_dst;
}EnumConvert_t;

#define enum_num_max 16
typedef struct
{
    uint16_t model_id;//模型ID
    ProtocolConvert_model_type_t model_type;//模型类型
    ProtocolConvert_date_type_t date_type;//数据类型
    uint16_t reg_addr;//寄存器地址
    uint8_t bit_field_msb;//位字段最高位
    uint8_t bit_field_lsb;//位字段最低位
    float factor;//因子
    float offset;//偏移量
    uint16_t enum_num;//枚举数量
    EnumConvert_t enum_convert[enum_num_max];//枚举转换
}ProtocolConvert_485_t;


typedef struct
{
    uint16_t model_id;//模型ID
    ProtocolConvert_model_type_t model_type;//模型类型
    ProtocolConvert_date_type_t date_type;//数据类型
    uint32_t frame_ID;//帧ID
    uint8_t frame_byte;//帧字节数
    uint8_t bit_field_msb;//位字段最高位
    uint8_t bit_field_lsb;//位字段最低位
    float factor;//因子
    float offset;//偏移量
    uint16_t enum_num;//枚举数量
    EnumConvert_t enum_convert[enum_num_max];//枚举转换
}ProtocolConvert_CAN_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef struct
{
    uint16_t start_addr;//起始地址
    uint16_t end_addr;//结束地址
    uint16_t cycle;//周期
    uint16_t cyclecnt;//周期计数器
}area_attr_t;

typedef struct
{
    ProtocolConvert_device_type_t device_type;//设备类型
    uint8_t device_no;//设备号
    uint8_t device_addr;//设备地址

    uint8_t area_num;//区域数量
    area_attr_t area_attr[10];//区域属性
}device_attr_t;

typedef struct
{
    uint32_t baud;//波特率
    uint8_t date_bit;//数据位
    uint8_t stop_bit;//停止位
    uint8_t parity;//校验位

    uint8_t *tx_buff;//发送缓冲区
    uint8_t *rx_buff;//接收缓冲区

    uint8_t device_num;//设备数量
    device_attr_t device_attr[3];//设备属性
}PortConfig_485_t;

typedef struct
{
    uint32_t baud;//波特率
}PortConfig_CAN_t;


typedef struct
{
    PortConfig_485_t _485[3];
    PortConfig_CAN_t can[3];
}PortConfig_t;
extern PortConfig_t PortConfig;







/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



#endif


