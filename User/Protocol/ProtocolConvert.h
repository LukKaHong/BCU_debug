#ifndef __PROTOCOL_CONVERT_H__
#define __PROTOCOL_CONVERT_H__

#include "main.h"


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
    uint16_t model_id;
    ProtocolConvert_model_type_t model_type;
    ProtocolConvert_date_type_t date_type;
    uint16_t reg_addr;
    uint8_t bit_field_msb;
    uint8_t bit_field_lsb;
    float factor;
    float offset;
    uint16_t enum_num;
    EnumConvert_t enum_convert[enum_num_max];
}ProtocolConvert_485_t;


typedef struct
{
    uint16_t model_id;
    ProtocolConvert_model_type_t model_type;
    ProtocolConvert_date_type_t date_type;
    uint32_t frame_ID;
    uint8_t frame_byte;
    uint8_t bit_field_msb;
    uint8_t bit_field_lsb;
    float factor;
    float offset;
    uint16_t enum_num;
    EnumConvert_t enum_convert[enum_num_max];
}ProtocolConvert_CAN_t;






#endif


