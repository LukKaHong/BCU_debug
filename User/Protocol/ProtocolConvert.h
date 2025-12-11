#ifndef __PROTOCOL_CONVERT_H__
#define __PROTOCOL_CONVERT_H__

#include "main.h"
#include "fdcan.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef enum
{
    PROTOCOL_MODBUS = 0,
    PROTOCOL_CAN    = 1,
    PROTOCOL_HongHaiSheng_Fire_CAN = 2,
}PROTOCOL_e;



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef enum
{
    DEVICE_TYPE_PCS       = 0,
    DEVICE_TYPE_Air       = 1,
    DEVICE_TYPE_Meter     = 2,
    DEVICE_TYPE_Fire      = 3,
    DEVICE_TYPE_Doil      = 4,
    DEVICE_TYPE_Pv        = 5,
    DEVICE_TYPE_Coolwater = 6,
    DEVICE_TYPE_Dehum     = 7,
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
#define enum_num_max (16)
#define modbus_area_num_max (10)
#define modbus_node_attr_num_max (128)

#define can_node_attr_num_max (128)

typedef struct
{
    uint16_t model_id;//模型ID
    MODEL_TYPE_e model_type;//模型类型
    DATE_TYPE_e date_type;//数据类型
    uint8_t fun_code;//功能码
    uint16_t reg_addr;//寄存器地址
    uint8_t bit_field_msb;//位字段最高位
    uint8_t bit_field_lsb;//位字段最低位
    float factor;//因子
    float offset;//偏移量
    uint16_t enum_num;//枚举数量
    EnumConvert_t enum_convert[enum_num_max];//枚举转换
}modbus_node_attr_t;

typedef struct
{
    uint8_t fun_code;//功能码
    uint16_t reg_addr;//寄存器地址
    uint16_t reg_num;//寄存器数量
    uint16_t cycle;//周期
}modbus_area_attr_t;

typedef struct
{
    uint8_t area_num;//区域数量
    modbus_area_attr_t area_attr[modbus_area_num_max];//区域属性
    uint16_t node_num;//点表数量
    modbus_node_attr_t node_attr[modbus_node_attr_num_max];//点表属性
}ProtocolConvert_modbus_t;

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
#define PortConfig_modbus_Num (3)
#define PortConfig_modbus_Device_Num (3)

typedef struct
{
    DEVICE_TYPE_e device_type;//设备类型
    PROTOCOL_e protocol;//协议
    uint8_t device_no;//设备号
    uint8_t device_addr;//设备地址
    
    /*----------------*/
    uint16_t cyclecnt[modbus_area_num_max];//周期计数器
}modbus_device_attr_t;

typedef struct
{
    uint32_t baud;//波特率
    uint8_t date_bit;//数据位
    uint8_t stop_bit;//停止位
    uint8_t parity;//校验位
    uint8_t device_num;//设备数量
    modbus_device_attr_t device_attr[PortConfig_modbus_Device_Num];//设备属性
}PortConfig_modbus_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define PortConfig_CAN_Num (3)
#define PortConfig_CAN_Device_Num (3)

typedef struct
{
    DEVICE_TYPE_e device_type;//设备类型
    PROTOCOL_e protocol;//协议
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
#define PortConfig_DI_Num (6)

typedef enum
{
    DI_Signal_water_logging = 0,
    DI_Signal_fire          = 1,
    DI_Signal_E_stop        = 2,
    DI_Signal_AC_break      = 3,
    DI_Signal_door          = 4,
    DI_Signal_cool_fan      = 5,
    DI_Signal_Max,
}DI_Signal_e;

typedef struct
{
    uint8_t en;
    DI_Signal_e signal;
    uint8_t valid;
    uint16_t trigger_delay;
    uint16_t recover_delay;
}PortConfig_DI_t;
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define PortConfig_DO_Num (6)

typedef enum
{
    DO_Ctrl_exhaust_fan = 0,
    DO_Ctrl_cool_fan    = 1,
    DO_Ctrl_heat        = 2,
    DO_Ctrl_red_led     = 3,
    DO_Ctrl_green_led   = 4,
    DO_Ctrl_Max,
}DO_Ctrl_e;

typedef struct
{
    uint8_t en;
    DO_Ctrl_e ctrl;
    uint8_t valid;
}PortConfig_DO_t;

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern ProtocolConvert_modbus_t* GetProtocolConvert_modbus(DEVICE_TYPE_e device_type);
extern ProtocolConvert_CAN_t* GetProtocolConvert_CAN(DEVICE_TYPE_e device_type);
extern PortConfig_modbus_t* GetPortConfig_modbus(uint8_t no);
extern PortConfig_CAN_t* GetPortConfig_CAN(uint8_t no);
extern PortConfig_DI_t* GetPortConfig_DI(uint8_t no);
extern PortConfig_DO_t* GetPortConfig_DO(uint8_t no);


extern void ConvertToNode_CAN(uint16_t* node, uint8_t* byte, CAN_node_attr_t* convert);
extern void ConvertToNode_modbus(uint16_t* node, uint8_t* byte, modbus_node_attr_t* convert);
extern uint32_t CAN_ID_offset_calc(uint32_t id, CAN_device_attr_t* device_attr);


extern void cJSON_To_PortConfig(char *message);
extern void cJSON_To_ProtocolConvert(char *message);
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



#endif


