#include "Comm485.h"
#include "cmsis_os.h"
#include "ProtocolConvert.h"
#include "ModbusRTU.h"
#include "ProtocolNode.h"
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t Comm485_1_Tx_Buff[Uart_Tx_Buff_Size];
uint8_t Comm485_1_Rx_Buff[Uart_Rx_Buff_Size];

uint8_t Comm485_2_Tx_Buff[Uart_Tx_Buff_Size];
uint8_t Comm485_2_Rx_Buff[Uart_Rx_Buff_Size];

uint8_t Comm485_3_Tx_Buff[Uart_Tx_Buff_Size];
uint8_t Comm485_3_Rx_Buff[Uart_Rx_Buff_Size];

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Comm_485_Read_Pro(uint8_t port, uint8_t *tx_buff, uint8_t *rx_buff)
{
    PortConfig_modbus_t* modbus = GetPortConfig_modbus(port);
    if(modbus == NULL)
        return;

    for(uint8_t device_num = 0; device_num < modbus->device_num; device_num++)//扫描所有设备
    {
        if(modbus->device_attr[device_num].protocol == PROTOCOL_MODBUS)
        {
            //获取协议
            ProtocolConvert_modbus_t* convert = GetProtocolConvert_modbus(modbus->device_attr[device_num].device_type);
            if(convert == NULL)
                continue;
        
            for(uint8_t area_num = 0; area_num < convert->area_num; area_num++)//扫描所有区域
            {
                if(++modbus->device_attr[device_num].cyclecnt[area_num] >= convert->area_attr[area_num].cycle / Comm485_Task_Cycle)//定时查询
                {
                    modbus->device_attr[device_num].cyclecnt[area_num] = 0;

                    uint16_t tx_len = 0;
                    const uint8_t* data;
                    uint16_t data_len;
                    int32_t result = MB_ERR_ARG;

                    if(convert->area_attr[area_num].fun_code == MB_FC_READ_COILS)
                    {
                        ModbusRTU_BuildReadCoils(
                            modbus->device_attr[device_num].device_addr,//设备地址
                            convert->area_attr[area_num].reg_addr,//寄存器地址
                            convert->area_attr[area_num].reg_num,//寄存器数量
                            tx_buff,
                            &tx_len
                        );
                        
                        result = ModbusRTU_ParseReadCoilsRsp(
                                rx_buff,
                                _485_Tx_And_Rx(port, tx_buff, tx_len, rx_buff, Uart_Rx_Buff_Size),
                                modbus->device_attr[device_num].device_addr,//设备地址
                                convert->area_attr[area_num].reg_num,//寄存器数量
                                &data,//存寄存器数据的指针
                                &data_len//字节数
                            );

                    }
                    else if(convert->area_attr[area_num].fun_code == MB_FC_READ_HOLDING_REGS)
                    {
                        ModbusRTU_BuildReadHolding(
                            modbus->device_attr[device_num].device_addr,//设备地址
                            convert->area_attr[area_num].reg_addr,//寄存器地址
                            convert->area_attr[area_num].reg_num,//寄存器数量
                            tx_buff,
                            &tx_len
                        );

                        result = ModbusRTU_ParseReadHoldingRsp(
                                rx_buff,
                                _485_Tx_And_Rx(port, tx_buff, tx_len, rx_buff, Uart_Rx_Buff_Size),
                                modbus->device_attr[device_num].device_addr,//设备地址
                                convert->area_attr[area_num].reg_num,//寄存器数量
                                &data,//存寄存器数据的指针
                                &data_len//字节数
                            );
                    }

                    if(result == MB_OK)
                    {
                        uint16_t reg_addr = convert->area_attr[area_num].reg_addr;

                        for(uint16_t reg_num = 0; reg_num < convert->area_attr[area_num].reg_num; reg_num++)//扫描读取出来的寄存器
                        {
                            for(uint16_t node_num = 0; node_num < convert->node_num; node_num++)//扫描所有点表
                            {
                                if(convert->node_attr[node_num].reg_addr == reg_addr && 
                                    convert->node_attr[node_num].fun_code == convert->area_attr[area_num].fun_code)//匹配点表
                                {
                                    uint16_t index = 0;

                                    if(ModelIdToNodeIndex(modbus->device_attr[device_num].device_type, modbus->device_attr[device_num].device_no, convert->node_attr[node_num].model_id, &index) == true)
                                    {
                                        ConvertToNode_modbus(GetNodePointer() + index, (uint8_t*)data, &convert->node_attr[node_num]);
                                        // printf("485 %d node[%d] = %d\n", port, index, GetNodePointer()[index]);
                                    }

                                    // break;
                                }
                            }
                            data += 2;//指针增加2字节
                            reg_addr++;//寄存器地址增加1
                        } 
                    }
                }
            }

        }
        else
        {


        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
static void Comm_485_Write_Pro(uint8_t port, uint8_t *tx_buff, uint8_t *rx_buff)
{
    PortConfig_modbus_t* modbus = GetPortConfig_modbus(port);
    if(modbus == NULL)
        return;

    if(modbus->en == 0)
        return;

    for(uint8_t device_num = 0; device_num < modbus->device_num; device_num++)//扫描所有设备
    {
        if(modbus->device_attr[device_num].protocol == PROTOCOL_MODBUS)
        {
            uint16_t start = 0, end = 0;
            if(GetNodeRange(modbus->device_attr[device_num].device_type, modbus->device_attr[device_num].device_no, &start, &end) == false)
                continue;

            Write_Node_t* write_node = GetWriteNodePointer();

            for(uint16_t index = start; index <= end; index++)//扫描所有点表
            {
                if(write_node->writeflag[index] == 1)//判断是否需要写入
                {
                    write_node->writeflag[index] = 0;

                    uint16_t model_id = 0;
                    if(NodeIndexToModelId(modbus->device_attr[device_num].device_type, modbus->device_attr[device_num].device_no, index, &model_id) == false)
                        continue;

                    uint16_t reg_addr = 0;
                    uint8_t fun_code = 0xff;
                    if(ModelIdToRegAddr_modbus(modbus->device_attr[device_num].device_type, model_id, &reg_addr, &fun_code) == false)
                        continue;

                    uint16_t tx_len = 0;
                    uint16_t rx_len = 0;
                    uint16_t value = 0;

                    if(fun_code == MB_FC_READ_HOLDING_REGS)
                    {
                        ModbusRTU_BuildWriteSingle(modbus->device_attr[device_num].device_addr, reg_addr, write_node->value[index], tx_buff, &tx_len);
                        rx_len = _485_Tx_And_Rx(port, tx_buff, tx_len, rx_buff, Uart_Rx_Buff_Size);
                        ModbusRTU_ParseWriteSingleRsp(rx_buff, rx_len, modbus->device_attr[device_num].device_addr, reg_addr, &value);
                    }
                }
            }
        }
        else
        {


        }
    }
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_1_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(Comm485_1_EventHandle, Comm485_1_Event_Tick | Comm485_1_Event_Write, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_1_Event_Tick)
        {
            // printf("%s\r\n", __func__);
            Comm_485_Read_Pro(1, Comm485_1_Tx_Buff, Comm485_1_Rx_Buff);

            // Comm485_1_Tx_Buff[0] = 0x01;
            // Comm485_1_Tx_Buff[1] = 0x03;
            // Comm485_1_Tx_Buff[2] = 0x00;
            // Comm485_1_Tx_Buff[3] = 0x0a;
            // Comm485_1_Tx_Buff[4] = 0x00;
            // Comm485_1_Tx_Buff[5] = 0x03;
            // Comm485_1_Tx_Buff[6] = 0x25;
            // Comm485_1_Tx_Buff[7] = 0xc9;
            // _485_Tx_And_Rx(1, Comm485_1_Tx_Buff, 8, Comm485_1_Rx_Buff, Uart_Rx_Buff_Size);
        }

        if(r_event & Comm485_1_Event_Write)
        {
            // Comm_485_Write_Pro(1, Comm485_1_Tx_Buff, Comm485_1_Rx_Buff);
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_2_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(Comm485_2_EventHandle, Comm485_2_Event_Tick | Comm485_2_Event_Write, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_2_Event_Tick)
        {
            // printf("%s\r\n", __func__);
            // Comm_485_Read_Pro(2, Comm485_2_Tx_Buff, Comm485_2_Rx_Buff);
            // _485_Tx_And_Rx(2, "222222", 6, Comm485_2_Rx_Buff, Uart_Rx_Buff_Size);
        }

        if(r_event & Comm485_2_Event_Write)
        {
            // Comm_485_Write_Pro(2, Comm485_2_Tx_Buff, Comm485_2_Rx_Buff);
        }
    }
}
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void Comm485_3_Task(void)
{
    while(1)
    {
        uint32_t r_event = osEventFlagsWait(Comm485_3_EventHandle, Comm485_3_Event_Tick | Comm485_3_Event_Write, osFlagsWaitAny, osWaitForever);

        if(r_event & Comm485_3_Event_Tick)
        {
            // printf("%s\r\n", __func__);
            // Comm_485_Read_Pro(3, Comm485_3_Tx_Buff, Comm485_3_Rx_Buff);
            // _485_Tx_And_Rx(3, "333333", 6, Comm485_3_Rx_Buff, Uart_Rx_Buff_Size);
        }

        if(r_event & Comm485_3_Event_Write)
        {
            // Comm_485_Write_Pro(3, Comm485_3_Tx_Buff, Comm485_3_Rx_Buff);
        }
    }
}






