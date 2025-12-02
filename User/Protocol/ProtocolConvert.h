#ifndef __PROTOCOL_CONVERT_H__
#define __PROTOCOL_CONVERT_H__

#include "main.h"
#include "fdcan.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



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
    DEVICE_TYPE_Max,
}DEVICE_TYPE_e;

typedef enum
{
    MODEL_TYPE_U16    = 0,
    MODEL_TYPE_S16    = 1,
    MODEL_TYPE_U32    = 2,
    MODEL_TYPE_S32    = 3,
}MODEL_TYPE_e;

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
}DATE_TYPE_e;

typedef struct
{
    uint16_t value_src;
    uint16_t value_dst;
}EnumConvert_t;

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define enum_num_max (8)
#define _485_area_num_max (10)
#define _485_node_attr_num_max (512)

#define can_node_attr_num_max (512)

typedef struct
{
    uint16_t model_id;//模型ID
    MODEL_TYPE_e model_type;//模型类型
    DATE_TYPE_e date_type;//数据类型
    uint16_t reg_addr;//寄存器地址
    uint8_t bit_field_msb;//位字段最高位
    uint8_t bit_field_lsb;//位字段最低位
    float factor;//因子
    float offset;//偏移量
    uint16_t enum_num;//枚举数量
    EnumConvert_t enum_convert[enum_num_max];//枚举转换
}_485_node_attr_t;

typedef struct
{
    uint16_t reg_addr;//寄存器地址
    uint16_t reg_num;//寄存器数量
    uint16_t cycle;//周期
}_485_area_attr_t;

typedef struct
{
    uint8_t area_num;//区域数量
    _485_area_attr_t area_attr[_485_area_num_max];//区域属性
    uint16_t node_num;//点表数量
    _485_node_attr_t node_attr[_485_node_attr_num_max];//点表属性
}ProtocolConvert_485_t;




typedef struct
{
    uint16_t model_id;//模型ID
    MODEL_TYPE_e model_type;//模型类型
    DATE_TYPE_e date_type;//数据类型
    uint32_t frame_ID;//帧ID
    uint8_t frame_byte;//帧字节数
    uint8_t bit_field_msb;//位字段最高位
    uint8_t bit_field_lsb;//位字段最低位
    float factor;//因子
    float offset;//偏移量
    uint16_t enum_num;//枚举数量
    EnumConvert_t enum_convert[enum_num_max];//枚举转换
}CAN_node_attr_t;


typedef struct
{
    uint16_t node_num;//点表数量
    CAN_node_attr_t node_attr[can_node_attr_num_max];//点表属性
}ProtocolConvert_CAN_t;
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define PortConfig_485_Num (3)
#define PortConfig_485_Device_Num (3)

#define PortConfig_CAN_Num (3)
#define PortConfig_CAN_Device_Num (3)

typedef struct
{
    DEVICE_TYPE_e device_type;//设备类型
    uint8_t device_no;//设备号
    uint8_t device_addr;//设备地址

    /*----------------*/
    uint16_t cyclecnt[_485_area_num_max];//周期计数器
}_485_device_attr_t;

typedef struct
{
    uint32_t baud;//波特率
    uint8_t date_bit;//数据位
    uint8_t stop_bit;//停止位
    uint8_t parity;//校验位
    uint8_t device_num;//设备数量
    _485_device_attr_t device_attr[PortConfig_485_Device_Num];//设备属性

}PortConfig_485_t;


typedef struct
{
    DEVICE_TYPE_e device_type;//设备类型
    uint8_t device_no;//设备号
    uint8_t master_addr;//主地址
    uint8_t slave_addr;//从地址
    uint8_t addr_format;//地址格式
}CAN_device_attr_t;

typedef struct
{
    uint32_t baud;//波特率
    uint8_t device_num;//设备数量
    CAN_device_attr_t device_attr[PortConfig_CAN_Device_Num];//设备属性
}PortConfig_CAN_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define PCS_Node_Max (1024)
#define PCS_Num_Max (2)

#define Air_Node_Max (1024)
#define Air_Num_Max (2)

#define Meter_Node_Max (1024)
#define Meter_Num_Max (2)

#define FIre_Node_Max (1024)
#define FIre_Num_Max (2)

#define Doil_Node_Max (1024)
#define Doil_Num_Max (2)

#define PvPCS_Node_Max (1024)
#define PvPCS_Num_Max (2)

typedef struct
{
    uint16_t Node[PCS_Node_Max];
}Node_PCS_t;

typedef struct
{
    uint16_t Node[Air_Node_Max];
}Node_Air_t;

typedef struct
{
    uint16_t Node[Meter_Node_Max];
}Node_Meter_t;

typedef struct
{
    uint16_t Node[FIre_Node_Max];
}Node_FIre_t;

typedef struct
{
    uint16_t Node[Doil_Node_Max];
}Node_Doil_t;

typedef struct
{
    uint16_t Node[PvPCS_Node_Max];
}Node_PvPCS_t;
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern void Comm_485_Pro(uint8_t port, uint8_t *tx_buff, uint8_t *rx_buff);
extern void Comm_CAN_Pro(uint8_t port, CanMsgType *msg);

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



#endif


