#include "CommLAN_2.h"
#include "cmsis_os.h"
#include "ModbusTCP.h"
#include "ModbusTCP_Client.h"
#include "ProtocolNode.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define bms_unit_id 0x01
static uint8_t trans_id = 0;


#define LAN_2_TxBuffer_Size (1 * 1024)
#define LAN_2_RxBuffer_Size (1 * 1024)

uint8_t LAN_2_TxBuffer[LAN_2_TxBuffer_Size];
uint8_t LAN_2_RxBuffer[LAN_2_RxBuffer_Size];
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void modbusTcp_Read_Reg_To_Node(uint8_t* tx, uint8_t* rx, uint16_t start_addr, uint16_t end_addr)
{
    if(start_addr > end_addr)
        return;

    uint16_t txlen = 0;
    uint16_t rxlen = 0;
    uint16_t quantity = end_addr - start_addr + 1;
    const uint8_t* data = NULL;
    uint16_t data_len = 0;

    if(ModbusTCP_BuildReadHolding(trans_id, bms_unit_id, start_addr, quantity, tx, &txlen) == MBTCP_OK)
    {
        
        // osSemaphoreAcquire(BinarySem_LAN_2_TxHandle, 0);
        // osSemaphoreAcquire(BinarySem_LAN_2_RxHandle, 0);
        //send
        // osSemaphoreAcquire(BinarySem_LAN_2_TxHandle, pdMS_TO_TICKS(1000));

        // osSemaphoreAcquire(BinarySem_LAN_2_RxHandle, pdMS_TO_TICKS(1000));


        if(ModbusTCP_ParseReadHoldingRsp(rx, rxlen, trans_id, bms_unit_id, quantity, &data, &data_len) == MBTCP_OK)
        {
            uint16_t* node = GetNodePointer();

            for(;data_len > 2 && start_addr < Node_Num_Max;)
            {
                *(GetNodePointer() + start_addr) = (uint16_t)((data[1] << 8) | data[0]);
                data += 2;
                data_len -= 2;
            }
        }  
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void LAN_EMS_Read_Pro(void)
{
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 10500, 10525);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 10600, 10656);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 12000, 12026);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 13000, 13099);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 13100, 13199);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 13200, 13299);
    modbusTcp_Read_Reg_To_Node(LAN_2_TxBuffer, LAN_2_RxBuffer, 13300, 13348);
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
            printf("%s\r\n", __func__);
        }
    }
}


