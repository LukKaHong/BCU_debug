#include "CommLAN_2.h"
#include "cmsis_os.h"
#include "ModbusTCP.h"
#include "ModbusTCP_Client.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define bms_unit_id 0x01
static uint8_t trans_id = 0;





/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void modbusTcp_Read_Reg(uint8_t* tx, uint8_t* rx, uint16_t* rxlen, uint16_t start_addr, uint16_t end_addr)
{
    if(start_addr > end_addr)
        return;

    uint16_t txlen = 0;

    if(ModbusTCP_BuildReadHolding(trans_id, bms_unit_id, start_addr, end_addr - start_addr + 1, tx, &txlen) == MBTCP_OK)
    {
        
        // osSemaphoreAcquire(BinarySem_LAN_2_TxHandle, 0);
        // osSemaphoreAcquire(BinarySem_LAN_2_RxHandle, 0);
        //send
        // osSemaphoreAcquire(BinarySem_LAN_2_TxHandle, pdMS_TO_TICKS(1000));

        // osSemaphoreAcquire(BinarySem_LAN_2_RxHandle, pdMS_TO_TICKS(1000));

        
    }


}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void LAN_EMS_Read_Pro(uint8_t* rx, uint16_t* rxlen, uint8_t* tx)
{
    uint16_t txlen = 0;

    // if(ModbusTCP_BuildReadHolding(trans_id, bms_unit_id, 10500, 10525 - 10500 + 1, tx, &txlen) == MBTCP_OK)
    // {
    //     ModbusTCP_ParseReadHoldingRsp(tx, txlen, 10500, 10525 - 10500 + 1);


    // }

    
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void CommLAN_2_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(CommLAN_2_EventHandle, CommLAN_2_Event_Tick, osFlagsWaitAny, osWaitForever);

        if(r_event & CommLAN_2_Event_Tick)
        {
            // printf("%s\r\n", __func__);
        }
    }
}


