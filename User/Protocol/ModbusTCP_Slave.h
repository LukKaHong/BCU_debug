#ifndef __MODBUS_TCP_SLAVE_H__
#define __MODBUS_TCP_SLAVE_H__

#include <stdint.h>
#include <stddef.h>
#include "ProtocolNode.h"

int32_t ModbusTCP_Slave_Handle(const uint8_t* req, uint16_t req_len,
                               uint8_t unit_id_expected,
                               uint16_t* holding_regs, uint16_t holding_count,
                               uint8_t* rsp, uint16_t* rsp_len,
                               Write_Node_t* write_node
                            );

#endif
