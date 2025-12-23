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





/* PCS 输出与状态定义 */
#define NODE_PCS_V_AB                  14600 // 输出AB线电压
#define NODE_PCS_V_BC                  14601 // 输出BC线电压
#define NODE_PCS_V_CA                  14602 // 输出CA线电压
#define NODE_PCS_V_A                   14603 // 输出A相电压
#define NODE_PCS_V_B                   14604 // 输出B相电压
#define NODE_PCS_V_C                   14605 // 输出C相电压
#define NODE_PCS_I_A                   14606 // 输出A相电流
#define NODE_PCS_I_B                   14607 // 输出B相电流
#define NODE_PCS_I_C                   14608 // 输出C相电流
#define NODE_PCS_IND_I_A               14609 // 电感A相电流
#define NODE_PCS_IND_I_B               14610 // 电感B相电流
#define NODE_PCS_IND_I_C               14611 // 电感C相电流
#define NODE_PCS_GRID_FREQ             14612 // 电网频率
#define NODE_PCS_GRID_PHASE_SEQ        14613 // 当前电网相序
#define NODE_PCS_DERATING_FACTOR       14614 // PCS降额系数
#define NODE_PCS_DERATING_FLAG         14615 // PCS降额标志
#define NODE_PCS_P_A                   14616 // 交流 A 相有功功率
#define NODE_PCS_P_B                   14617 // 交流 B 相有功功率
#define NODE_PCS_P_C                   14618 // 交流 C 相有功功率
#define NODE_PCS_P_TOTAL               14619 // 交流输出总有功功率
#define NODE_PCS_S_A                   14620 // 交流 A 相视在功率
#define NODE_PCS_S_B                   14621 // 交流 B 相视在功率
#define NODE_PCS_S_C                   14622 // 交流 C 相视在功率
#define NODE_PCS_S_TOTAL               14623 // 交流输出总视在功率
#define NODE_PCS_Q_A                   14624 // 交流 A 相无功功率
#define NODE_PCS_Q_B                   14625 // 交流 B 相无功功率
#define NODE_PCS_Q_C                   14626 // 交流 C 相无功功率
#define NODE_PCS_Q_TOTAL               14627 // 交流输出总无功功率
#define NODE_PCS_PF_A                  14628 // A 相功率因数
#define NODE_PCS_PF_B                  14629 // B 相功率因数
#define NODE_PCS_PF_C                  14630 // C 相功率因数
#define NODE_PCS_PF_TOTAL              14631 // 交流功率因数
#define NODE_PCS_BUS_V_TOTAL           14632 // 总母线电压
#define NODE_PCS_BUS_V_POS             14633 // 正母线电压
#define NODE_PCS_BUS_V_NEG             14634 // 负母线电压
#define NODE_PCS_BAT_V                 14635 // 电池电压
#define NODE_PCS_BAT_I                 14636 // 电池电流
#define NODE_PCS_DC_P                  14637 // 直流功率
#define NODE_PCS_DC_I_TOTAL            14638 // 直流总电流
#define NODE_PCS_IGBT_TEMP             14639 // IGBT 温度
#define NODE_PCS_ENV_TEMP              14640 // 环境温度
#define NODE_PCS_IND_TEMP              14641 // 电感温度
#define NODE_PCS_WORK_STATUS           14642 // 工作状态
#define NODE_PCS_CHARGE_MODE           14643 // 当前实际充放电工作模式

/* PCS 电量统计 (双寄存器) */
#define NODE_PCS_DC_CHARGE_HIST_H      14644 // 直流历史充电量 H
#define NODE_PCS_DC_CHARGE_HIST_L      14645 // 直流历史充电量 L
#define NODE_PCS_DC_CHARGE_DAY_H       14646 // 直流日充电量 H
#define NODE_PCS_DC_CHARGE_DAY_L       14647 // 直流日充电量 L
#define NODE_PCS_DC_DISCHARGE_HIST_H   14648 // 直流历史放电量 H
#define NODE_PCS_DC_DISCHARGE_HIST_L   14649 // 直流历史放电量 L
#define NODE_PCS_DC_DISCHARGE_DAY_H    14650 // 直流日放电量 H
#define NODE_PCS_DC_DISCHARGE_DAY_L    14651 // 直流日放电量 L
#define NODE_PCS_AC_CHARGE_HIST_H      14652 // 交流历史充电量 H
#define NODE_PCS_AC_CHARGE_HIST_L      14653 // 交流历史充电量 L
#define NODE_PCS_AC_CHARGE_DAY_H       14654 // 交流日充电量 H
#define NODE_PCS_AC_CHARGE_DAY_L       14655 // 交流日充电量 L
#define NODE_PCS_AC_DISCHARGE_HIST_H   14656 // 交流历史放电量 H
#define NODE_PCS_AC_DISCHARGE_HIST_L   14657 // 交流历史放电量 L
#define NODE_PCS_AC_DISCHARGE_DAY_H    14658 // 交流日放电量 H
#define NODE_PCS_AC_DISCHARGE_DAY_L    14659 // 交流日放电量 L
#define NODE_PCS_FEED_GRID_H           14660 // 馈网电量 H
#define NODE_PCS_FEED_GRID_L           14661 // 馈网电量 L
#define NODE_PCS_GRID_SUPPLY_H         14662 // 电网供电电量 H
#define NODE_PCS_GRID_SUPPLY_L         14663 // 电网供电电量 L

#define NODE_PCS_MODULE_MAX_TEMP       14664 // 模块最高温度
#define NODE_PCS_ACTIVE_POWER_CMD      14665 // 有功功率调度
#define NODE_PCS_REACTIVE_POWER_CMD    14666 // 无功功率期望



/* PCS 故障与报警定义 */
#define NODE_PCS_COMM_FAULT                15000 // 通讯故障
#define NODE_PCS_INV_FAULT                 15001 // 逆变故障
#define NODE_PCS_N_LINE_ABNORMAL           15002 // N线异常
#define NODE_PCS_PARALLEL_FAULT            15003 // 并机故障
#define NODE_PCS_SYNC_SIGNAL_FAULT         15004 // 同步信号故障
#define NODE_PCS_RELAY_FAULT               15005 // 继电器故障
#define NODE_PCS_WAVE_LIMIT                15006 // 逐波限流
#define NODE_PCS_INV_SOFT_START_FAIL       15007 // 逆变软起失败
#define NODE_PCS_AC_SHORT_CIRCUIT          15008 // AC短路故障
#define NODE_PCS_AC_OVERLOAD_PROT          15009 // AC过载保护
#define NODE_PCS_VF_AC_V_HIGH              15010 // (VF)AC输出电压高
#define NODE_PCS_VF_AC_V_LOW               15011 // (VF)AC输出电压低
#define NODE_PCS_VF_AC_FREQ_LOW            15012 // (VF)AC输出频率低
#define NODE_PCS_BUS_UNBALANCE             15013 // 母线不平衡
#define NODE_PCS_BUS_OVER_VOLT             15014 // 母线过压
#define NODE_PCS_BUS_UNDER_VOLT            15015 // 母线欠压
#define NODE_PCS_BUS_HW_ABNORMAL           15016 // 母线硬件异常
#define NODE_PCS_DC_SOFT_START_FAIL        15017 // DC软起失败
#define NODE_PCS_DC_REVERSE                15018 // DC反接
#define NODE_PCS_DC_INPUT_OVER_VOLT        15019 // DC输入过压
#define NODE_PCS_DC_INPUT_UNDER_VOLT       15020 // DC输入欠压
#define NODE_PCS_DC1_OVER_CURR             15021 // 第一路DC过流
#define NODE_PCS_DC2_OVER_CURR             15022 // 第二路DC过流
#define NODE_PCS_DC_HW_OVER_CURR           15023 // DC硬件过流
#define NODE_PCS_PV_HW_OVER_CURR           15024 // PV硬件过流(08H)
#define NODE_PCS_UNBALANCE_OVER_CURR       15025 // 不平衡过流
#define NODE_PCS_IGBT_OVER_TEMP            15026 // IGBT过温
#define NODE_PCS_DC_CURR_UNBALANCE         15027 // DC电流不平衡
#define NODE_PCS_DC_CONTACTOR_FAULT        15028 // DC接触器故障
#define NODE_PCS_GRID_FREQ_LOW             15029 // 电网频率低
#define NODE_PCS_GRID_FREQ_HIGH            15030 // 电网频率高
#define NODE_PCS_GRID_VOLT_LOW             15031 // 电网电压低
#define NODE_PCS_GRID_VOLT_HIGH            15032 // 电网电压高
#define NODE_PCS_GRID_REVERSE_SEQ          15033 // 电网反序
#define NODE_PCS_GRID_RECOVERY_FLAG        15034 // 电网恢复标志
#define NODE_PCS_VOLT_FAST_CHECK_ERR       15035 // 电压快检异常
#define NODE_PCS_ISLANDING_FAULT           15036 // 孤岛故障
#define NODE_PCS_CURRENT_OVERLOAD          15037 // 电流过流
#define NODE_PCS_N_LINE_CURR_OVERLOAD      15038 // N线电流过流
#define NODE_PCS_IND_CURR_OVERLOAD         15039 // 电感电流过流
#define NODE_PCS_LVRT_FAULT                15040 // 低压穿越故障
#define NODE_PCS_HVRT_FAULT                15041 // 高压穿越故障
#define NODE_PCS_GRID_NORMAL_FLAG          15042 // 电网正常标志
#define NODE_PCS_LEAKAGE_CURR_ALARM        15043 // 漏电流告警
#define NODE_PCS_LIGHTNING_FAULT           15044 // 防雷故障
#define NODE_PCS_FAN_ALARM                 15045 // 风扇告警
#define NODE_PCS_INSULATION_FAULT_I        15046 // 绝缘故障I
#define NODE_PCS_MODE_ERROR                15047 // 模式错误
#define NODE_PCS_AUX_POWER_FAULT           15048 // 辅助电源故障
#define NODE_PCS_SYSTEM_FAULT              15049 // 系统故障
#define NODE_PCS_COM_BOARD_FAULT           15050 // COM通信板故障
#define NODE_PCS_BMS_FAULT                 15051 // BMS故障
#define NODE_PCS_MODULE_OVER_TEMP          15052 // 模块过温
#define NODE_PCS_IGBT_TEMP_ABNORMAL        15053 // IGBT温度异常
#define NODE_PCS_EXT_FLASH_ERR             15054 // 外部FLASH异常
#define NODE_PCS_BMS_COMM_INT_ALARM        15055 // BMS通信中断告警
#define NODE_PCS_INTERNAL_COMM_FAULT       15056 // 内部通信故障
#define NODE_PCS_MCAN_ERR                  15057 // MCAN异常
#define NODE_PCS_PCAN_ERR                  15058 // PCAN异常
#define NODE_PCS_CPLD_ERR                  15059 // CPLD异常
#define NODE_PCS_COMM_FAULT_2              15060 // 通讯故障
#define NODE_PCS_OVERLOAD_ALARM            15061 // 正在过载告警
#define NODE_PCS_WAVE_LIMIT_ALARM          15062 // 逐波限流告警
#define NODE_PCS_LIMIT_RUN_FLAG            15063 // 机器限额运行标志
#define NODE_PCS_PV1_VOLT_LOW              15064 // 光伏1电压低
#define NODE_PCS_PV2_VOLT_LOW              15065 // 光伏2电压低
#define NODE_PCS_PV1_VOLT_HIGH             15066 // 光伏1电压高
#define NODE_PCS_PV2_VOLT_HIGH             15067 // 光伏2电压高
#define NODE_PCS_PV1_INPUT_REVERSE         15068 // 光伏1输入反接
#define NODE_PCS_PV2_INPUT_REVERSE         15069 // 光伏2输入反接
#define NODE_PCS_PV1_OVER_CURR             15070 // PV1过流
#define NODE_PCS_PV2_OVER_CURR             15071 // PV2过流
#define NODE_PCS_EPO_FAULT_FLAG            15072 // EPO 故障标志
#define NODE_PCS_IGBT_HW_OVER_CURR_FLAG    15073 // IGBT 硬件过流标志
#define NODE_PCS_BUS_HW_OVER_VOLT_FLAG     15074 // 母线硬件过压标志
#define NODE_PCS_POWER_MOD_WAVE_LIMIT_FLAG 15075 // 功率模块逐波限流标志
#define NODE_PCS_BAL_MOD_HW_OVER_CURR_FLAG 15076 // 平衡模块硬件过流标志
#define NODE_PCS_24V_POWER_FAULT_FLAG      15077 // 24V 电源故障标志
#define NODE_PCS_FAN_FAULT_FLAG            15078 // 风扇故障标志
#define NODE_PCS_CONN_FAULT_FLAG           15079 // 连接故障标志
#define NODE_PCS_LIGHTNING_ARRESTER_FAULT  15080 // 防雷器故障
#define NODE_PCS_IND_OVER_TEMP_FAULT_FLAG  15081 // 电感过温故障标志
#define NODE_PCS_POWER_MOD_OVER_TEMP_FLAG  15082 // 功率模块过温标志
#define NODE_PCS_BAL_MOD_OVER_TEMP_FLAG    15083 // 平衡模块过温标志
#define NODE_PCS_15V_POWER_FAULT_FLAG      15084 // 15V 电源故障标志
#define NODE_PCS_SYS_FIRE_FAULT_FLAG       15085 // 系统火警故障标志
#define NODE_PCS_BAT_DRY_CONTACT_FAULT_FLAG 15086 // 电池干接点故障标志
#define NODE_PCS_DRY_CONTACT_OVERLOAD_FAULT_FLAG 15087 // 干接点过载故障标志
#define NODE_PCS_ENV_OVER_TEMP_FAULT_FLAG  15088 // 环境温度过温故障标志
#define NODE_PCS_DRY_CONTACT_OVER_TEMP_FAULT_FLAG 15089 // 干接点过温故障标志
#define NODE_PCS_A_OVER_VOLT_FAULT_FLAG    15090 // A 相过压故障标志
#define NODE_PCS_B_OVER_VOLT_FAULT_FLAG    15091 // B 相过压故障标志
#define NODE_PCS_C_OVER_VOLT_FAULT_FLAG    15092 // C 相过压故障标志
#define NODE_PCS_A_UNDER_VOLT_FAULT_FLAG   15093 // A 相欠压故障标志
#define NODE_PCS_B_UNDER_VOLT_FAULT_FLAG   15094 // B 相欠压故障标志
#define NODE_PCS_C_UNDER_VOLT_FAULT_FLAG   15095 // C 相欠压故障标志
#define NODE_PCS_GRID_OVER_FREQ            15096 // 电网过频
#define NODE_PCS_GRID_UNDER_FREQ           15097 // 电网欠频
#define NODE_PCS_GRID_PHASE_SEQ_ERR        15098 // 电网相序错误
#define NODE_PCS_A_SW_OVER_CURR            15099 // A 相软件过流
#define NODE_PCS_B_SW_OVER_CURR            15100 // B 相软件过流
#define NODE_PCS_C_SW_OVER_CURR            15101 // C 相软件过流
#define NODE_PCS_GRID_VOLT_UNBALANCE       15102 // 电网电压不平衡
#define NODE_PCS_GRID_CURR_UNBALANCE       15103 // 电网电流不平衡
#define NODE_PCS_GRID_PHASE_LOSS           15104 // 电网缺相
#define NODE_PCS_N_LINE_OVER_CURR          15105 // N 线过流
#define NODE_PCS_PRECHARGE_BUS_OVER_VOLT   15106 // 预充母线过压
#define NODE_PCS_PRECHARGE_BUS_UNDER_VOLT  15107 // 预充母线欠压
#define NODE_PCS_UNCTRL_RECT_BUS_OVER_VOLT 15108 // 不控整流母线过压
#define NODE_PCS_UNCTRL_RECT_BUS_UNDER_VOLT 15109 // 不控整流母线欠压
#define NODE_PCS_RUN_BUS_OVER_VOLT         15110 // 运行母线过压
#define NODE_PCS_RUN_BUS_UNDER_VOLT        15111 // 运行母线欠压
#define NODE_PCS_POS_NEG_BUS_UNBALANCE     15112 // 正负母线不平衡
#define NODE_PCS_BAT_UNDER_VOLT            15113 // 电池欠压
#define NODE_PCS_CURR_MODE_BUS_UNDER_VOLT  15114 // 电流模式母线欠压
#define NODE_PCS_BAT_OVER_VOLT             15115 // 电池过压
#define NODE_PCS_DC_PRECHARGE_OVER_CURR    15116 // 直流预充电过流
#define NODE_PCS_DC_OVER_CURR              15117 // 直流过流
#define NODE_PCS_BAL_MOD_SW_OVER_CURR      15118 // 平衡模块软件过流
#define NODE_PCS_BAT_REVERSE               15119 // 电池反接
#define NODE_PCS_PRECHARGE_TIMEOUT         15120 // 预充电超时
#define NODE_PCS_PRECHARGE_A_OVER_CURR     15121 // 预充电A相过流
#define NODE_PCS_PRECHARGE_B_OVER_CURR     15122 // 预充电B相过流
#define NODE_PCS_PRECHARGE_C_OVER_CURR     15123 // 预充电C相过流
#define NODE_PCS_CTRL_BOARD_EEPROM_FAULT   15124 // 控制板 EEPROM 故障
#define NODE_PCS_AD_SAMPLE_ZERO_DRIFT      15125 // AD 采样零漂故障
#define NODE_PCS_BACKEND_PROTO_FAULT       15126 // 后台通讯协议故障
#define NODE_PCS_INSULATION_CHECK_FAULT    15127 // 绝缘检测故障
#define NODE_PCS_BMS_BAT_SYS_FAULT         15128 // BMS 电池系统故障
#define NODE_PCS_STS_COMM_FAULT            15129 // STS 通信故障
#define NODE_PCS_BMS_COMM_FAULT            15130 // BMS 通讯故障
#define NODE_PCS_EMS_COMM_FAULT            15131 // EMS 通讯故障
#define NODE_PCS_CAN_COMM_FAULT            15132 // CAN 通信故障
#define NODE_PCS_PRECHARGE_RELAY_CLOSE_FAIL 15133 // 预充电继电器闭合失败
#define NODE_PCS_PRECHARGE_RELAY_OPEN_FAIL 15134 // 预充电继电器断开失败
#define NODE_PCS_PRECHARGE_RELAY_CLOSE_ERR 15135 // 预充电继电器闭合状态错误
#define NODE_PCS_PRECHARGE_RELAY_OPEN_ERR  15136 // 预充电继电器断开状态错误
#define NODE_PCS_MAIN_RELAY_CLOSE_FAIL     15137 // 主继电器闭合失败
#define NODE_PCS_MAIN_RELAY_OPEN_FAIL      15138 // 主继电器断开失败
#define NODE_PCS_MAIN_RELAY_CLOSE_ERR      15139 // 主继电器闭合状态错误
#define NODE_PCS_MAIN_RELAY_OPEN_ERR       15140 // 主继电器断开状态错误
#define NODE_PCS_AC_MAIN_RELAY_ADHESION    15141 // 交流主继电器粘连故障
#define NODE_PCS_DC_RELAY_OPEN_FAULT       15142 // 直流继电器开路故障
#define NODE_PCS_INV_V_A_OVER_VOLT         15143 // 逆变电压 A 相过压故障标志
#define NODE_PCS_INV_V_B_OVER_VOLT         15144 // 逆变电压 B 相过压故障标志
#define NODE_PCS_INV_V_C_OVER_VOLT         15145 // 逆变电压 C 相过压故障标志
#define NODE_PCS_GRID_ISLAND_FAULT         15146 // 电网孤岛故障标志
#define NODE_PCS_SYS_RESONANCE_FAULT       15147 // 系统谐振故障标志
#define NODE_PCS_SW_OVER_VOLT_CURR         15148 // 软件过压过流标志
#define NODE_PCS_HVRT_TIMEOUT_FAULT        15149 // 高电压穿越超时故障标志
#define NODE_PCS_INV_V_A_UNDER_VOLT        15150 // 逆变电压 A 相欠压故障标志
#define NODE_PCS_INV_V_B_UNDER_VOLT        15151 // 逆变电压 B 相欠压故障标志
#define NODE_PCS_INV_V_C_UNDER_VOLT        15152 // 逆变电压 C 相欠压故障标志
#define NODE_PCS_OFF_GRID_NO_SYNC_FAULT    15153 // 离网无同步信号故障标志
#define NODE_PCS_OFF_GRID_SHORT_FAULT      15154 // 离网短路故障标志
#define NODE_PCS_LVRT_TIMEOUT_FAULT        15155 // 低电压穿越超时故障标志






/* PCS 参数设置 */
#define NODE_PCS_COM_BAUDRATE               16000 // COM波特率
#define NODE_PCS_SCI_STOP_BIT               16001 // SCI停止位
#define NODE_PCS_SCI_PARITY                 16002 // SCI奇偶校验位
#define NODE_PCS_EMS_PROTO_SEL              16003 // EMS协议选择
#define NODE_PCS_BMS_TIMEOUT                16004 // BMS超时时间
#define NODE_PCS_EPO_ENABLE                 16005 // EPO使能
#define NODE_PCS_BMS_MS_SEL                 16006 // BMS主从机选择
#define NODE_PCS_BMS_PROTO_SEL              16007 // BMS协议选择
#define NODE_PCS_POWER_SCHED_BMS_LINK       16008 // 功率调度关联BMS使能
#define NODE_PCS_VOLT_CALIB_ZERO            16009 // 电压校零
#define NODE_PCS_OUTPUT_VOLT_SYSTEM         16010 // 输出电压体制
#define NODE_PCS_GRID_POWER_SCHED_MODE      16011 // 并网功率调度方式
#define NODE_PCS_AC_CONST_CURR_REF          16012 // 交流侧恒电流期望值
#define NODE_PCS_DC_CONST_POWER_REF         16013 // 直流侧恒功率期望
#define NODE_PCS_DC_CONST_CURR_REF          16014 // 直流侧恒电流期望
#define NODE_PCS_GRID_OVER_VOLT_I_AMP       16015 // 电网过压I段保护幅值
#define NODE_PCS_GRID_OVER_VOLT_I_TIME      16016 // 电网过压I段保护时间
#define NODE_PCS_GRID_OVER_VOLT_II_AMP      16017 // 电网过压Ⅱ段保护幅值
#define NODE_PCS_GRID_OVER_VOLT_II_TIME     16018 // 电网过压Ⅱ段保护时间
#define NODE_PCS_GRID_UNDER_VOLT_I_AMP      16019 // 电网欠压I段保护幅值
#define NODE_PCS_GRID_UNDER_VOLT_I_TIME     16020 // 电网欠压I段保护时间
#define NODE_PCS_GRID_UNDER_VOLT_II_AMP     16021 // 电网欠压Ⅱ段保护幅值
#define NODE_PCS_GRID_UNDER_VOLT_II_TIME    16022 // 电网欠压Ⅱ段保护时间
#define NODE_PCS_GRID_UNDER_VOLT_III_AMP    16023 // 电网欠压Ⅲ段保护幅值
#define NODE_PCS_GRID_UNDER_VOLT_III_TIME   16024 // 电网欠压Ⅲ段保护时间
#define NODE_PCS_GRID_OVER_FREQ_I_AMP       16025 // 电网过频I段保护幅值
#define NODE_PCS_GRID_OVER_FREQ_I_TIME      16026 // 电网过频I段保护时间
#define NODE_PCS_GRID_OVER_FREQ_II_AMP      16027 // 电网过频Ⅱ段保护幅值
#define NODE_PCS_GRID_OVER_FREQ_II_TIME     16028 // 电网过频Ⅱ段保护时间
#define NODE_PCS_GRID_UNDER_FREQ_I_AMP      16029 // 电网欠频I段保护幅值
#define NODE_PCS_GRID_UNDER_FREQ_I_TIME     16030 // 电网欠频I段保护时间
#define NODE_PCS_GRID_UNDER_FREQ_II_AMP     16031 // 电网欠频Ⅱ段保护幅值
#define NODE_PCS_GRID_UNDER_FREQ_II_TIME    16032 // 电网欠频Ⅱ段保护时间
#define NODE_PCS_ISLANDING_DETECT           16033 // 孤岛检测
#define NODE_PCS_POWER_RAMP_RATE            16034 // 功率变化速率
#define NODE_PCS_COMM_TIMEOUT_ACTION        16035 // 通信超时关机/待机
#define NODE_PCS_ACTIVE_POWER_CTRL_MODE     16036 // 有功三相总控/分控
#define NODE_PCS_ACTIVE_POWER_A             16037 // A相有功
#define NODE_PCS_ACTIVE_POWER_B             16038 // B相有功
#define NODE_PCS_ACTIVE_POWER_C             16039 // C相有功
#define NODE_PCS_REACTIVE_POWER_A           16040 // A相无功
#define NODE_PCS_REACTIVE_POWER_B           16041 // B相无功
#define NODE_PCS_REACTIVE_POWER_C           16042 // C相无功
#define NODE_PCS_REGULATION_COEFF            16043 // 调节系数
#define NODE_PCS_VOLT_RIDE_THROUGH_EN        16044 // 电压穿越功能使能
#define NODE_PCS_REACTIVE_POWER_ADJ_MODE     16045 // 无功功率调整模式
#define NODE_PCS_INSULATION_DETECT_EN        16046 // 绝缘检测使能
#define NODE_PCS_VOLT_DROOP                  16047 // 电压下垂
#define NODE_PCS_FREQ_DROOP                  16048 // 频率下垂
#define NODE_PCS_VSG_TIME_CONST              16049 // VSG时间常数
#define NODE_PCS_VSG_DAMPING_COEFF           16050 // VSG阻尼系数
#define NODE_PCS_VSG_SEC_REG_ACTIVE_POWER    16051 // VSG二次调节有功功率
#define NODE_PCS_VSG_SEC_REG_REACTIVE_POWER  16052 // VSG二次调节无功功率
#define NODE_PCS_REACTIVE_COMP_POWER_SET     16053 // 无功功率补偿功率设置
#define NODE_PCS_OFF_GRID_MODE_SET           16054 // 离网模式设置
#define NODE_PCS_MODULE_RUN_MODE_SET         16055// 模块运行模式设置
#define NODE_PCS_GRID_SINGLE_PHASE_CTRL      16056// 并网单相功率独立控制设置
#define NODE_PCS_MODULE_MASTER_SET           16057// 模块主机设置
#define NODE_PCS_MODULE_CURR_SOURCE_PARALLEL 16058// 模块电流源并联使能设置
#define NODE_PCS_OFF_GRID_VOLT_SET           16059// 离网电压设置
#define NODE_PCS_OFF_GRID_FREQ_SET           16060// 离网频率设置
#define NODE_PCS_A_ACTIVE_POWER_REF          16061// A 相有功功率给定
#define NODE_PCS_A_REACTIVE_POWER_REF        16062// A 相无功功率给定
#define NODE_PCS_B_ACTIVE_POWER_REF          16063// B 相有功功率给定
#define NODE_PCS_B_REACTIVE_POWER_REF        16064// B 相无功功率给定
#define NODE_PCS_C_ACTIVE_POWER_REF          16065// C 相有功功率给定
#define NODE_PCS_C_REACTIVE_POWER_REF        16066// C 相无功功率给定
#define NODE_PCS_GRID_OVER_FREQ_PROT         16067// 电网过频保护
#define NODE_PCS_GRID_UNDER_FREQ_PROT       16068// 电网欠频保护
#define NODE_PCS_ISLAND_OVER_FREQ_PROT      16069// 孤岛过频保护
#define NODE_PCS_ISLAND_UNDER_FREQ_PROT     16070// 孤岛欠频保护
#define NODE_PCS_BAT_OVER_VOLT_PROT         16071// 电池过压保护点
#define NODE_PCS_BAT_UNDER_VOLT_PROT        16072// 电池欠压保护点
#define NODE_PCS_GRID_MAX_CHARGE_DISCHARGE_CURR 16073// 并网允许最大充电/放电电流
#define NODE_PCS_BUS_OVER_VOLT_PROT        16074// 母线过压保护点
#define NODE_PCS_BUS_UNDER_VOLT_PROT         16075// 母线欠压保护点
#define NODE_PCS_DC_OVER_CURR_PROT         16076// 直流过流保护点
#define NODE_PCS_AC_LINE_OVER_VOLT_PROT      16077// 交流线电压过压保护点
#define NODE_PCS_AC_LINE_UNDER_VOLT_PROT     16078// 交流线电压欠压保护点
#define NODE_PCS_AC_OVER_CURR_PROT         16079// 交流过流保护点
#define NODE_PCS_MODULE_OVER_TEMP_PROT       16080// 模块过温保护点
#define NODE_PCS_CC_CV_SWITCH_VOLT           16081// 恒流转恒压充电电池电压点
#define NODE_PCS_BAT_CV_CHARGE_VOLT          16082// 电池恒压充电电压
#define NODE_PCS_CV_CHARGE_MAX_TIME          16083// 恒压充电最大时间限制
#define NODE_PCS_CV_FLOAT_SWITCH_CURR        16084// 恒压转浮充电流点设置
#define NODE_PCS_BAT_FLOAT_VOLT              16085// 电池浮充电压点设置
#define NODE_PCS_FLOAT_CC_SWITCH_CURR        16086// 浮充转恒流电流点设置
#define NODE_PCS_REMODBUS_ADDR               16087// ReModbus 地址
#define NODE_PCS_REMODBUS_BAUDRATE           16088// ReModbus 波特率
#define NODE_PCS_HMI_MODBUS_ADDR             16089// HMI Modbus 地址
#define NODE_PCS_HMI_MODBUS_BAUDRATE         16090// HMI Modbus 波特率
#define NODE_PCS_EMS_BMS_COMM_FAULT_TIME     16091// EMS(BMS)通讯故障时间阈值



#define NODE_PCS_CMD_POWER_ON                16300// 开机
#define NODE_PCS_CMD_POWER_OFF               16301// 关机
#define NODE_PCS_CMD_GRID_CONNECT            16302// 并网
#define NODE_PCS_CMD_GRID_DISCONNECT         16303// 离网
#define NODE_PCS_CMD_SAVE_PARAM              16304// 保存参数
#define NODE_PCS_CMD_PARAM_INIT              16305// 参数初始化
#define NODE_PCS_CMD_POWER_SET               16306// 下发功率
#define NODE_PCS_CMD_STANDBY                 16307// 待机
#define NODE_PCS_CMD_FAULT_RESET             16308// 故障复位
#define NODE_PCS_CMD_PARAM_SOLIDIFY          16309// 固化参数
















/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef struct
{
    uint16_t value[Node_Num_Max];
    NODE_TYPE_e type[Node_Num_Max];
}Node_t;


typedef struct
{
    uint16_t value[Node_Num_Max];
    uint8_t writeflag[Node_Num_Max];
}Write_Node_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern Node_t* GetNodePointer(void);
extern uint16_t* GetNodeValuePointer(void);
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


