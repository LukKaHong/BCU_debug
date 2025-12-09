#include "ModbusTCP.h"
#include "ModbusTCP_Client.h"

static inline void put16_be(uint8_t* dst, uint16_t val)
{
    dst[0] = (uint8_t)(val >> 8);
    dst[1] = (uint8_t)(val & 0xFF);
}

static inline uint16_t get16_be(const uint8_t* src)
{
    return (uint16_t)(((uint16_t)src[0] << 8) | src[1]);
}

static inline int32_t check_mbap(const uint8_t* frame, uint16_t len, uint16_t* trans_id_out, uint16_t* proto_id_out, uint16_t* mbap_len_out, uint8_t* unit_id_out)
{
    if (frame == NULL || len < 8) return MBTCP_ERR_LEN;
    uint16_t tid = get16_be(&frame[0]);
    uint16_t pid = get16_be(&frame[2]);
    uint16_t mlen = get16_be(&frame[4]);
    uint8_t uid = frame[6];
    if (proto_id_out) *proto_id_out = pid;
    if (trans_id_out) *trans_id_out = tid;
    if (mbap_len_out) *mbap_len_out = mlen;
    if (unit_id_out) *unit_id_out = uid;
    if (pid != 0) return MBTCP_ERR_PROTO;
    if ((uint16_t)(len - 6) != (uint16_t)(mlen + 1)) return MBTCP_ERR_LEN;
    return MBTCP_OK;
}

int32_t ModbusTCP_BuildReadHolding(uint16_t trans_id, uint8_t unit_id,
                                   uint16_t start, uint16_t quantity,
                                   uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL || quantity == 0 || quantity > 125) return MBTCP_ERR_ARG;
    put16_be(&frame[0], trans_id);
    put16_be(&frame[2], 0);
    put16_be(&frame[4], 6);
    frame[6] = unit_id;
    frame[7] = MBTCP_FC_READ_HOLDING_REGS;
    put16_be(&frame[8], start);
    put16_be(&frame[10], quantity);
    *frame_len = 12;
    return MBTCP_OK;
}

int32_t ModbusTCP_BuildWriteSingle(uint16_t trans_id, uint8_t unit_id,
                                   uint16_t reg, uint16_t value,
                                   uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL) return MBTCP_ERR_ARG;
    put16_be(&frame[0], trans_id);
    put16_be(&frame[2], 0);
    put16_be(&frame[4], 6);
    frame[6] = unit_id;
    frame[7] = MBTCP_FC_WRITE_SINGLE_REG;
    put16_be(&frame[8], reg);
    put16_be(&frame[10], value);
    *frame_len = 12;
    return MBTCP_OK;
}

int32_t ModbusTCP_BuildWriteMultiple(uint16_t trans_id, uint8_t unit_id,
                                     uint16_t start, const uint16_t* values, uint16_t quantity,
                                     uint8_t* frame, uint16_t* frame_len)
{
    if (frame == NULL || frame_len == NULL || values == NULL || quantity == 0 || quantity > 123) return MBTCP_ERR_ARG;
    uint8_t bc = (uint8_t)(quantity * 2);
    put16_be(&frame[0], trans_id);
    put16_be(&frame[2], 0);
    put16_be(&frame[4], (uint16_t)(7 + bc));
    frame[6] = unit_id;
    frame[7] = MBTCP_FC_WRITE_MULTIPLE_REGS;
    put16_be(&frame[8], start);
    put16_be(&frame[10], quantity);
    frame[12] = bc;
    for (uint16_t i = 0; i < quantity; i++) put16_be(&frame[13 + i * 2], values[i]);
    *frame_len = (uint16_t)(13 + bc);
    return MBTCP_OK;
}

int32_t ModbusTCP_ParseReadHoldingRsp(const uint8_t* frame, uint16_t len,
                                      uint16_t expected_trans_id, uint8_t expected_unit_id, uint16_t expected_quantity,
                                      const uint8_t** data, uint16_t* data_len)
{
    uint16_t tid=0,pid=0,ml=0; uint8_t uid=0;
    int32_t st = check_mbap(frame, len, &tid, &pid, &ml, &uid);
    if (st != MBTCP_OK) return st;
    if (expected_trans_id && tid != expected_trans_id) return MBTCP_ERR_DATA;
    if (expected_unit_id && uid != expected_unit_id) return MBTCP_ERR_DATA;
    uint8_t func = frame[7];
    if (func == (uint8_t)(MBTCP_FC_READ_HOLDING_REGS | 0x80)) return MBTCP_ERR_EXC;
    if (func != MBTCP_FC_READ_HOLDING_REGS) return MBTCP_ERR_FUNC;
    uint8_t bc = frame[8];
    if ((uint16_t)(len) != (uint16_t)(7 + 2 + bc)) return MBTCP_ERR_LEN;
    if ((bc & 1U) != 0U) return MBTCP_ERR_DATA;
    if (expected_quantity && bc != expected_quantity * 2) return MBTCP_ERR_DATA;
    *data = &frame[9];
    *data_len = bc;
    return MBTCP_OK;
}

int32_t ModbusTCP_ParseWriteSingleRsp(const uint8_t* frame, uint16_t len,
                                      uint16_t expected_trans_id, uint8_t expected_unit_id, uint16_t expected_reg,
                                      uint16_t* value)
{
    uint16_t tid=0,pid=0,ml=0; uint8_t uid=0;
    int32_t st = check_mbap(frame, len, &tid, &pid, &ml, &uid);
    if (st != MBTCP_OK) return st;
    if (expected_trans_id && tid != expected_trans_id) return MBTCP_ERR_DATA;
    if (expected_unit_id && uid != expected_unit_id) return MBTCP_ERR_DATA;
    uint8_t func = frame[7];
    if (func == (uint8_t)(MBTCP_FC_WRITE_SINGLE_REG | 0x80)) return MBTCP_ERR_EXC;
    if (func != MBTCP_FC_WRITE_SINGLE_REG) return MBTCP_ERR_FUNC;
    if (len != 12) return MBTCP_ERR_LEN;
    uint16_t reg = get16_be(&frame[8]);
    if (expected_reg != 0xFFFF && reg != expected_reg) return MBTCP_ERR_DATA;
    *value = get16_be(&frame[10]);
    return MBTCP_OK;
}

int32_t ModbusTCP_ParseWriteMultipleRsp(const uint8_t* frame, uint16_t len,
                                        uint16_t expected_trans_id, uint8_t expected_unit_id,
                                        uint16_t* start, uint16_t* quantity)
{
    uint16_t tid=0,pid=0,ml=0; uint8_t uid=0;
    int32_t st = check_mbap(frame, len, &tid, &pid, &ml, &uid);
    if (st != MBTCP_OK) return st;
    if (expected_trans_id && tid != expected_trans_id) return MBTCP_ERR_DATA;
    if (expected_unit_id && uid != expected_unit_id) return MBTCP_ERR_DATA;
    uint8_t func = frame[7];
    if (func == (uint8_t)(MBTCP_FC_WRITE_MULTIPLE_REGS | 0x80)) return MBTCP_ERR_EXC;
    if (func != MBTCP_FC_WRITE_MULTIPLE_REGS) return MBTCP_ERR_FUNC;
    if (len != 12) return MBTCP_ERR_LEN;
    *start = get16_be(&frame[8]);
    *quantity = get16_be(&frame[10]);
    return MBTCP_OK;
}

