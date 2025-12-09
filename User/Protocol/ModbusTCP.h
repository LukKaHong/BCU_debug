#ifndef __MODBUS_TCP_H__
#define __MODBUS_TCP_H__

#include <stdint.h>
#include <stddef.h>

#define MBTCP_FC_READ_HOLDING_REGS   0x03
#define MBTCP_FC_WRITE_SINGLE_REG    0x06
#define MBTCP_FC_WRITE_MULTIPLE_REGS 0x10

#define MBTCP_OK        0
#define MBTCP_ERR_ARG  -1
#define MBTCP_ERR_LEN  -2
#define MBTCP_ERR_PROTO -3
#define MBTCP_ERR_FUNC -4
#define MBTCP_ERR_DATA -5
#define MBTCP_ERR_EXC  -6

#define MBTCP_EX_ILLEGAL_FUNCTION  0x01
#define MBTCP_EX_ILLEGAL_DATA_ADDR 0x02
#define MBTCP_EX_ILLEGAL_DATA_VAL  0x03
#define MBTCP_EX_SLAVE_FAILURE     0x04

#include "ModbusTCP_Client.h"
#include "ModbusTCP_Slave.h"

#endif
