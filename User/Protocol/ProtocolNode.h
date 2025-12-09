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
#define ECU_Num_Max (1)
#define ECU_Node_Max (500)

#define Air_Num_Max (1)
#define Air_Node_Max (1000)

#define Fire_Num_Max (2)
#define Fire_Node_Max (1000)

#define Meter_Num_Max (2)
#define Meter_Node_Max (1000)

#define Pv_Num_Max (2)
#define Pv_Node_Max (1000)

#define Doil_Num_Max (1)
#define Doil_Node_Max (1000)

#define Coolwater_Num_Max (1)
#define Coolwater_Node_Max (1000)

#define Dehum_Num_Max (1)
#define Dehum_Node_Max (1000)

#define BMS_Num_Max (1)
#define BMS_Node_Max (4000)

#define PCS_Num_Max (2)
#define PCS_Node_Max (2000)

#define BMU_Num_Max (20)
#define BMU_Node_Max (1000)


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
    uint16_t Node[Fire_Node_Max];
}Node_Fire_t;

typedef struct
{
    uint16_t Node[Doil_Node_Max];
}Node_Doil_t;

typedef struct
{
    uint16_t Node[Pv_Node_Max];
}Node_Pv_t;

typedef struct
{
    uint16_t Node[Coolwater_Node_Max];
}Node_Coolwater_t;

typedef struct
{
    uint16_t Node[Dehum_Node_Max];
}Node_Dehum_t;

typedef struct
{
    uint16_t Node[ECU_Node_Max];
}Node_ECU_t;

typedef struct
{
    uint16_t Node[BMS_Node_Max];
}Node_BMS_t;

typedef struct
{
    uint16_t Node[BMU_Node_Max];
}Node_BMU_t;




/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
extern uint16_t* GetNode(DEVICE_TYPE_e device_type, uint8_t no);

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#endif


