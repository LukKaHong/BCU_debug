#ifndef _LANMGR_IF_H_
#define _LANMGR_IF_H_

//#include "Public.h"
#include "LANMgr.h"

//Íø¿Ú2È¡1±àÒë
#define LAN_W5500 1
#define LAN_LWIP  0

extern lwip_dev lwipdev;
extern w5500_dev w5500dev;

void LANMgrInit(void);
void LANECS_task(void *pvParameters);
void LANUPPER_task(void *pvParameters);
void LANTest_task(void *pvParameters);


#endif
