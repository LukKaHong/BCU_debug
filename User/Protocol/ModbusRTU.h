#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#include <stdint.h>
#include <stddef.h>

#define MB_FC_READ_HOLDING_REGS   0x03
#define MB_FC_WRITE_SINGLE_REG    0x06
#define MB_FC_WRITE_MULTIPLE_REGS 0x10

#define MB_OK        0
#define MB_ERR_ARG  -1
#define MB_ERR_LEN  -2
#define MB_ERR_CRC  -3
#define MB_ERR_ADDR -4
#define MB_ERR_FUNC -5
#define MB_ERR_DATA -6
#define MB_ERR_EXC  -7

uint16_t ModbusRTU_CRC16(const uint8_t* data, uint16_t len);
int32_t  ModbusRTU_Validate(const uint8_t* frame, uint16_t len);

int32_t  ModbusRTU_BuildReadHolding(uint8_t addr, uint16_t start, uint16_t quantity,
                                    uint8_t* frame, uint16_t* frame_len);

int32_t  ModbusRTU_BuildWriteSingle(uint8_t addr, uint16_t reg, uint16_t value,
                                    uint8_t* frame, uint16_t* frame_len);

int32_t  ModbusRTU_BuildWriteMultiple(uint8_t addr, uint16_t start,
                                      const uint16_t* values, uint16_t quantity,
                                      uint8_t* frame, uint16_t* frame_len);

int32_t  ModbusRTU_ParseReadHoldingRsp(const uint8_t* frame, uint16_t len,
                                       uint8_t expected_addr, uint16_t expected_quantity,
                                       const uint8_t** data, uint16_t* data_len);

int32_t  ModbusRTU_ParseWriteSingleRsp(const uint8_t* frame, uint16_t len,
                                       uint8_t expected_addr, uint16_t expected_reg,
                                       uint16_t* value);

int32_t  ModbusRTU_ParseWriteMultipleRsp(const uint8_t* frame, uint16_t len,
                                         uint8_t expected_addr, uint16_t* start,
                                         uint16_t* quantity);

#endif

