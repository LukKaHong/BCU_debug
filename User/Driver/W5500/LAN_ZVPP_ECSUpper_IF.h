#ifndef _LAN_ZVPP_ECSUPPER_IF_H_
#define _LAN_ZVPP_ECSUPPER_IF_H_

#include "LAN_ZVPP_ECSUpper.h"
//#include "ModelBcuApi.h"

extern tPowerTest PowerTest;
extern tPCSPowerSTRinfo PCSPowerSTRinfo;
//extern St_BmuConfData BmuConfDataTemp[BMUMAXAMOUNT];

void LAN_Pro(void);
void LAN_W5500_Pro(void);
void LAN_ZVPP_ECSUpper_Init(void);
void LAN_ZVPP_ECS_Connect(void);
void LAN_ZVPP_Upper_Connect(void);

void CloseAllSocket(void);

#endif
