#ifndef __SYS_MANAGE_H__
#define __SYS_MANAGE_H__

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#include "main.h"
#include "ProtocolNode.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define SysManage_Task_Cycle (100)


#define SysFaultLv_0        0
#define SysFaultLv_1        1
#define SysFaultLv_2        2
#define SysFaultLv_3        3
#define SysFaultLv_4        4
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef struct
{
    uint8_t En;
    uint8_t Fault;
    uint8_t Rxflag;
    uint32_t Cnt;
    uint32_t Timeout;
}Comm_Fault_t;

typedef struct
{
    Comm_Fault_t Air[Air_Num_Max];
    Comm_Fault_t Fire[Fire_Num_Max];
    Comm_Fault_t Meter[Meter_Num_Max];
    Comm_Fault_t Pv[Pv_Num_Max];
    Comm_Fault_t Doil[Doil_Num_Max];
    Comm_Fault_t Coolwater[Coolwater_Num_Max];
    Comm_Fault_t Dehum[Dehum_Num_Max];
    Comm_Fault_t BMS[BMS_Num_Max];
    Comm_Fault_t PCS[PCS_Num_Max];
}Device_Comm_t;

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
typedef enum
{
    PCS_Status_unkown  = 0,
    PCS_Status_fault   = 1,
    PCS_Status_standby = 2,
    PCS_Status_run     = 3,
    PCS_Status_stop    = 4,
}PCS_Status_t;

typedef enum
{
    BMS_Status_unkown    = 0,
    BMS_Status_poweron   = 1,
    BMS_Status_powerdown = 2,
}BMS_Status_t;

typedef enum
{
    Grid_Setting_unkown  = 0,
    Grid_Setting_Off     = 1,
    Grid_Setting_connect = 2,
}Grid_Status_t;

typedef struct
{
    uint8_t SysFaultLv;
    BMS_Status_t BMS_Status;
    
    int16_t setpower[PCS_Num_Max];

    PCS_Status_t PCS_Status[PCS_Num_Max];
    PCS_Status_t PCS_Setting[PCS_Num_Max];

    Grid_Status_t Grid_Status[PCS_Num_Max];
    Grid_Status_t Grid_Setting[PCS_Num_Max];
}SysInfo_t;


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern void SysManage_Task(void);

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

#endif




