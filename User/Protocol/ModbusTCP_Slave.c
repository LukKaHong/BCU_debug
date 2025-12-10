#include "ModbusTCP.h"
#include "ModbusTCP_Slave.h"



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

static inline int32_t build_exception(uint16_t tid, uint8_t uid, uint8_t func, uint8_t ex,
                                      uint8_t* rsp, uint16_t* rsp_len)
{
    if (rsp == NULL || rsp_len == NULL) return MBTCP_ERR_ARG;
    put16_be(&rsp[0], tid);
    put16_be(&rsp[2], 0);
    put16_be(&rsp[4], 3);
    rsp[6] = uid;
    rsp[7] = (uint8_t)(func | 0x80);
    rsp[8] = ex;
    *rsp_len = 9;
    return MBTCP_OK;
}

/*
 * 从机请求处理函数（保持寄存器）
 * 参数说明：
 *  req            : 接收到的完整请求帧（包含 MBAP 头 + PDU）
 *  req_len        : 请求帧总长度，必须等于 7 + MBAP.Length 字节
 *  unit_id_expected : 期望的 UnitID（为 0 表示接受任意 UnitID）
 *  holding_regs   : 保持寄存器数组（uint16_t，作为寄存器映射的后端存储）
 *  holding_count  : 保持寄存器数组的元素个数（寄存器数量）
 *  rsp            : 输出响应缓冲区（函数成功时填充正常或异常响应）
 *  rsp_len        : 输出响应长度（字节数，函数成功时写入）
 * 返回值：
 *  MBTCP_OK       : 已生成响应（正常或异常），应将 rsp 的前 rsp_len 字节回发
 *  其他 MBTCP_ERR_*：校验或参数错误，通常不需要回发响应
 */
int32_t ModbusTCP_Slave_Handle(const uint8_t* req, uint16_t req_len,
                               uint8_t unit_id_expected,
                               uint16_t* holding_regs, uint16_t holding_count,
                               uint8_t* rsp, uint16_t* rsp_len,
                               Write_Node_t* write_node
                            )
{
    uint16_t tid=0,pid=0,ml=0; uint8_t uid=0;
    if (holding_regs == NULL || rsp == NULL || rsp_len == NULL) return MBTCP_ERR_ARG;
    int32_t st = check_mbap(req, req_len, &tid, &pid, &ml, &uid);
    if (st != MBTCP_OK) return st;
    if (unit_id_expected && uid != unit_id_expected) return MBTCP_ERR_DATA;
    if (req_len < 8) return MBTCP_ERR_LEN;
    uint8_t func = req[7];
    if (func == MBTCP_FC_READ_HOLDING_REGS)
    {
        if (req_len != 12) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        uint16_t start = get16_be(&req[8]);
        uint16_t qty   = get16_be(&req[10]);
        if (qty == 0 || qty > 125) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        if ((uint32_t)start + qty > holding_count) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_ADDR, rsp, rsp_len);
        uint8_t bc = (uint8_t)(qty * 2);
        put16_be(&rsp[0], tid);
        put16_be(&rsp[2], 0);
        put16_be(&rsp[4], (uint16_t)(3 + bc));
        rsp[6] = uid;
        rsp[7] = func;
        rsp[8] = bc;
        for (uint16_t i = 0; i < qty; i++) put16_be(&rsp[9 + i*2], holding_regs[start + i]);
        *rsp_len = (uint16_t)(9 + bc);
        return MBTCP_OK;
    }
    else if (func == MBTCP_FC_WRITE_SINGLE_REG)
    {
        if (req_len != 12) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        uint16_t reg = get16_be(&req[8]);
        uint16_t val = get16_be(&req[10]);
        if (reg >= holding_count) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_ADDR, rsp, rsp_len);
        // holding_regs[reg] = val;
        write_node->value[reg] = val;
        write_node->writeflag[reg] = 1;

        put16_be(&rsp[0], tid);
        put16_be(&rsp[2], 0);
        put16_be(&rsp[4], 6);
        rsp[6] = uid;
        rsp[7] = func;
        put16_be(&rsp[8], reg);
        put16_be(&rsp[10], val);
        *rsp_len = 12;
        return MBTCP_OK;
    }
    else if (func == MBTCP_FC_WRITE_MULTIPLE_REGS)
    {
        if (req_len < 13) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        uint16_t start = get16_be(&req[8]);
        uint16_t qty   = get16_be(&req[10]);
        uint8_t bc = req[12];
        if (qty == 0 || qty > 123) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        if (bc != (uint8_t)(qty * 2)) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        if ((uint32_t)start + qty > holding_count) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_ADDR, rsp, rsp_len);
        if (req_len != (uint16_t)(13 + bc)) return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_DATA_VAL, rsp, rsp_len);
        for (uint16_t i = 0; i < qty; i++)
        {
            // holding_regs[start + i] = get16_be(&req[13 + i*2]);
            write_node->value[start + i] = get16_be(&req[13 + i*2]);
            write_node->writeflag[start + i] = 1;
        }
        put16_be(&rsp[0], tid);
        put16_be(&rsp[2], 0);
        put16_be(&rsp[4], 6);
        rsp[6] = uid;
        rsp[7] = func;
        put16_be(&rsp[8], start);
        put16_be(&rsp[10], qty);
        *rsp_len = 12;
        return MBTCP_OK;
    }
    else
    {
        return build_exception(tid, uid, func, MBTCP_EX_ILLEGAL_FUNCTION, rsp, rsp_len);
    }
}
