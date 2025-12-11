#ifndef __PROTOCOL_NODE_H__
#define __PROTOCOL_NODE_H__

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#include "main.h"
#include "ProtocolConvert.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define Node_Num_Max (38500)

#define Node_Addr_ECU (0)
#define Node_Addr_Air (500)
#define Node_Addr_Fire (1500)
#define Node_Addr_Meter (3500)
#define Node_Addr_Pv (5500)
#define Node_Addr_Doil (7500)
#define Node_Addr_Coolwater (8500)
#define Node_Addr_Dehum (9500)
#define Node_Addr_BMS (10500)
#define Node_Addr_PCS (14500)
#define Node_Addr_BMU (18500)

#define Node_Num_ECU (500)
#define Node_Num_Air (1000)
#define Node_Num_Fire (1000)
#define Node_Num_Meter (1000)
#define Node_Num_Pv (1000)
#define Node_Num_Doil (1000)
#define Node_Num_Coolwater (1000)
#define Node_Num_Dehum (1000)
#define Node_Num_BMS (4000)
#define Node_Num_PCS (2000)
#define Node_Num_BMU (1000)

#define ECU_Num_Max (1)
#define Air_Num_Max (1)
#define Fire_Num_Max (2)
#define Meter_Num_Max (2)
#define Pv_Num_Max (2)
#define Doil_Num_Max (1)
#define Coolwater_Num_Max (1)
#define Dehum_Num_Max (1)
#define BMS_Num_Max (1)
#define PCS_Num_Max (2)
#define BMU_Num_Max (20)
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/* 传感器与状态定义 */
#define NODE_Fire_SENSOR_STATUS             1600 // 传感器状态
#define NODE_Fire_TEMP_VALUE                1601 // 温度
#define NODE_Fire_CO_VALUE                  1602 // CO数值
#define NODE_Fire_SMOKE_LIQUID_VALUE        1603 // 气敏烟雾（漏液）数值
#define NODE_Fire_SMOKE_PHOTO_VALUE         1604 // 光电烟雾数值
#define NODE_Fire_BATTERY_DOOR_STATUS       1605 // 电池仓门状态
#define NODE_Fire_WATER_LEAK_STATUS         1606 // 水浸报警状态
#define NODE_Fire_LIGHTNING_FEEDBACK        1607 // 防雷反馈信号
#define NODE_Fire_VALVE_STATUS              1608 // 瓶头阀状态
#define NODE_Fire_PACK_FIRE_FEEDBACK        1609 // Pack消防反馈信号
#define NODE_Fire_ELEC_FAN_STATUS           1610 // 电气仓风扇状态
#define NODE_Fire_DIST_BOX_TEMP             1611 // 配电仓温度

/* 故障与报警定义 */
#define NODE_Fire_COMM_FAULT                2000 // 通讯故障
#define NODE_Fire_ALARM_STATUS              2001 // 报警状态
#define NODE_Fire_SENSOR_SMOKE_PHOTO_ALARM  2002 // 传感器光电烟雾
#define NODE_Fire_SENSOR_CO_ALARM           2003 // 传感器一氧化碳
#define NODE_Fire_SENSOR_HIGH_TEMP_ALARM    2004 // 传感器高温
#define NODE_Fire_SENSOR_LIQUID_LEAK_ALARM  2005 // 传感器电解液泄露
#define NODE_Fire_ELEC_FAN_FAULT            2006 // 电气仓风扇故障



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef struct
{
    uint16_t value[Node_Num_Max];
    uint8_t writeflag[Node_Num_Max];
}Write_Node_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern uint16_t* GetNodePointer(void);
extern Write_Node_t* GetWriteNodePointer(void);
extern bool GetNodeRange(DEVICE_TYPE_e device_type, uint8_t no,uint16_t* start, uint16_t* end);

extern bool NodeIndexToModelId(DEVICE_TYPE_e device_type, uint8_t no, uint16_t index, uint16_t* model_id);
extern bool ModelIdToRegAddr_modbus(DEVICE_TYPE_e device_type, uint16_t model_id, uint16_t* reg_addr, uint8_t* fun_code);
extern bool ModelIdToNodeIndex(DEVICE_TYPE_e device_type, uint8_t no, uint16_t model_id, uint16_t* index);

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#endif


