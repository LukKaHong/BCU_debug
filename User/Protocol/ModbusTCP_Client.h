#ifndef __MODBUS_TCP_CLIENT_H__
#define __MODBUS_TCP_CLIENT_H__

#include <stdint.h>
#include <stddef.h>

int32_t ModbusTCP_BuildReadHolding(uint16_t trans_id, uint8_t unit_id,
                                   uint16_t start, uint16_t quantity,
                                   uint8_t* frame, uint16_t* frame_len);

int32_t ModbusTCP_BuildWriteSingle(uint16_t trans_id, uint8_t unit_id,
                                   uint16_t reg, uint16_t value,
                                   uint8_t* frame, uint16_t* frame_len);

int32_t ModbusTCP_BuildWriteMultiple(uint16_t trans_id, uint8_t unit_id,
                                     uint16_t start, const uint16_t* values, uint16_t quantity,
                                     uint8_t* frame, uint16_t* frame_len);

int32_t ModbusTCP_ParseReadHoldingRsp(const uint8_t* frame, uint16_t len,
                                      uint16_t expected_trans_id, uint8_t expected_unit_id, uint16_t expected_quantity,
                                      const uint8_t** data, uint16_t* data_len);

int32_t ModbusTCP_ParseWriteSingleRsp(const uint8_t* frame, uint16_t len,
                                      uint16_t expected_trans_id, uint8_t expected_unit_id, uint16_t expected_reg,
                                      uint16_t* value);

int32_t ModbusTCP_ParseWriteMultipleRsp(const uint8_t* frame, uint16_t len,
                                        uint16_t expected_trans_id, uint8_t expected_unit_id,
                                        uint16_t* start, uint16_t* quantity);

#endif
