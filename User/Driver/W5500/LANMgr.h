#ifndef _LANMGR_H
#define _LANMGR_H
#include "stdint.h"

typedef struct  
{
	uint8_t mac[6];      	//MAC地址
	uint8_t ip[4];       	//本机IP地址
	uint8_t netmask[4]; 	//子网掩码
	uint8_t gateway[4]; 	//默认网关的IP地址
}lwip_dev;


typedef struct  
{
	uint8_t mac[6];      	//MAC地址
	uint8_t ip[4];       	//本机IP地址
	uint8_t netmask[4]; 	//子网掩码
	uint8_t gateway[4]; 	//默认网关的IP地址
}w5500_dev;


#endif
