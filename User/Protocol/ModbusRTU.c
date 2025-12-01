#include "ModbusRTU.h"

static inline void put16_be(uint8_t* dst, uint16_t val)
{
    dst[0] = (uint8_t)(val >> 8);
    dst[1] = (uint8_t)(val & 0xFF);
}

static inline uint16_t get16_be(const uint8_t* src)
{
    return (uint16_t)(((uint16_t)src[0] << 8) | src[1]);
}

uint16_t ModbusRTU_CRC16(const uint8_t* data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++)
    {
        crc ^= (uint16_t)data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 1U) crc = (uint16_t)((crc >> 1) ^ 0xA001);
            else crc = (uint16_t)(crc >> 1);
        }
    }
    return crc;
}

int32_t ModbusRTU_Validate(const uint8_t* frame, uint16_t len)
{
    if (frame == NULL || len < 4) return MB_ERR_LEN;
    uint16_t crc = ModbusRTU_CRC16(frame, (uint16_t)(len - 2));
    uint16_t rxcrc = (uint16_t)(frame[len - 2] | ((uint16_t)frame[len - 1] << 8));
    return (crc == rxcrc) ? MB_OK : MB_ERR_CRC;
}

int32_t ModbusRTU_BuildReadHolding(uint8_t addr, uint16_t start, uint16_t quantity,
                                    uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL || quantity == 0 || quantity > 125) return MB_ERR_ARG;
    frame[0] = addr;
    frame[1] = MB_FC_READ_HOLDING_REGS;
    put16_be(&frame[2], start);
    put16_be(&frame[4], quantity);
    uint16_t crc = ModbusRTU_CRC16(frame, 6);
    frame[6] = (uint8_t)(crc & 0xFF);
    frame[7] = (uint8_t)(crc >> 8);
    *frame_len = 8;
    return MB_OK;
}

int32_t ModbusRTU_BuildWriteSingle(uint8_t addr, uint16_t reg, uint16_t value,
                                    uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL) return MB_ERR_ARG;
    frame[0] = addr;
    frame[1] = MB_FC_WRITE_SINGLE_REG;
    put16_be(&frame[2], reg);
    put16_be(&frame[4], value);
    uint16_t crc = ModbusRTU_CRC16(frame, 6);
    frame[6] = (uint8_t)(crc & 0xFF);
    frame[7] = (uint8_t)(crc >> 8);
    *frame_len = 8;
    return MB_OK;
}

int32_t ModbusRTU_BuildWriteMultiple(uint8_t addr, uint16_t start,
                                      const uint16_t* values, uint16_t quantity,
                                      uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL || values == NULL || quantity == 0 || quantity > 123) return MB_ERR_ARG;
    frame[0] = addr;
    frame[1] = MB_FC_WRITE_MULTIPLE_REGS;
    put16_be(&frame[2], start);
    put16_be(&frame[4], quantity);
    uint8_t byte_count = (uint8_t)(quantity * 2);
    frame[6] = byte_count;
    for (uint16_t i = 0; i < quantity; i++)
    {
        put16_be(&frame[7 + i * 2], values[i]);
    }
    uint16_t data_len = (uint16_t)(7 + byte_count);
    uint16_t crc = ModbusRTU_CRC16(frame, data_len);
    frame[data_len] = (uint8_t)(crc & 0xFF);
    frame[data_len + 1] = (uint8_t)(crc >> 8);
    *frame_len = (uint16_t)(data_len + 2);
    return MB_OK;
}

int32_t ModbusRTU_ParseReadHoldingRsp(const uint8_t* frame, uint16_t len,
                                       uint8_t expected_addr, uint16_t expected_quantity,
                                       const uint8_t** data, uint16_t* data_len)
{
    if (frame == NULL || data == NULL || data_len == NULL) return MB_ERR_ARG;
    if (ModbusRTU_Validate(frame, len) != MB_OK) return MB_ERR_CRC;
    if (len < 5) return MB_ERR_LEN;
    if (frame[0] != expected_addr) return MB_ERR_ADDR;
    uint8_t func = frame[1];
    if (func == (uint8_t)(MB_FC_READ_HOLDING_REGS | 0x80)) return MB_ERR_EXC;
    if (func != MB_FC_READ_HOLDING_REGS) return MB_ERR_FUNC;
    uint8_t bc = frame[2];
    if (len != (uint16_t)(3 + bc + 2)) return MB_ERR_LEN;
    if ((bc & 1U) != 0U) return MB_ERR_DATA;
    if (expected_quantity != 0 && bc != expected_quantity * 2) return MB_ERR_DATA;
    *data = &frame[3];
    *data_len = bc;
    return MB_OK;
}

int32_t ModbusRTU_ParseWriteSingleRsp(const uint8_t* frame, uint16_t len,
                                       uint8_t expected_addr, uint16_t expected_reg,
                                       uint16_t* value)
{
    if (frame == NULL || value == NULL) return MB_ERR_ARG;
    if (ModbusRTU_Validate(frame, len) != MB_OK) return MB_ERR_CRC;
    if (len != 8) return MB_ERR_LEN;
    if (frame[0] != expected_addr) return MB_ERR_ADDR;
    uint8_t func = frame[1];
    if (func == (uint8_t)(MB_FC_WRITE_SINGLE_REG | 0x80)) return MB_ERR_EXC;
    if (func != MB_FC_WRITE_SINGLE_REG) return MB_ERR_FUNC;
    uint16_t reg = get16_be(&frame[2]);
    if (expected_reg != 0xFFFF && reg != expected_reg) return MB_ERR_DATA;
    *value = get16_be(&frame[4]);
    return MB_OK;
}

int32_t ModbusRTU_ParseWriteMultipleRsp(const uint8_t* frame, uint16_t len,
                                         uint8_t expected_addr, uint16_t* start,
                                         uint16_t* quantity)
{
    if (frame == NULL || start == NULL || quantity == NULL) return MB_ERR_ARG;
    if (ModbusRTU_Validate(frame, len) != MB_OK) return MB_ERR_CRC;
    if (len != 8) return MB_ERR_LEN;
    if (frame[0] != expected_addr) return MB_ERR_ADDR;
    uint8_t func = frame[1];
    if (func == (uint8_t)(MB_FC_WRITE_MULTIPLE_REGS | 0x80)) return MB_ERR_EXC;
    if (func != MB_FC_WRITE_MULTIPLE_REGS) return MB_ERR_FUNC;
    *start = get16_be(&frame[2]);
    *quantity = get16_be(&frame[4]);
    return MB_OK;
}

