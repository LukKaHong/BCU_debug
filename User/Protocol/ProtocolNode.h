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
extern uint16_t* GetNode(DEVICE_TYPE_e device_type, uint8_t no, uint16_t model_id);
extern uint16_t* GetNodePointer(void);


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#endif


