#ifndef _LAN_ZVPP_ECSUPPER_H
#define _LAN_ZVPP_ECSUPPER_H
#include "stdint.h"









/*
|------------------0x0800 0000------------------
|
|
|bootload-128k-0x0002 0000
|
|
|------------------0x0801 ffff------------------
|------------------0x0802 0000------------------
|
|
|app-512k-0x0008 0000
|
|
|------------------0x0809 ffff------------------
|------------------0x080A 0000------------------
|
|
|para1-128k-0x0002 0000
|
|
|------------------0X080B FFFF------------------
|------------------0X080C 0000------------------
|
|
|para2-128k-0x0002 0000
|
|
|------------------0X080D FFFF------------------
|------------------0x080E 0000------------------
*/


#define Bootloader_FLASH_START_ADDR		(0x08000000)
#define Bootloader_FLASH_END_ADDR		(0x08020000-1)
#define Bootloader_FLASH_SIZE			(Bootloader_FLASH_END_ADDR - Bootloader_FLASH_START_ADDR + 1)

#define App_FLASH_START_ADDR			(0x08020000)
#define App_FLASH_END_ADDR				(0x080A0000-1)
#define App_FLASH_SIZE					(App_FLASH_END_ADDR - App_FLASH_START_ADDR + 1)

#define Para_1_FLASH_START_ADDR			(0x080A0000)
#define Para_1_FLASH_END_ADDR			(0X080C0000-1)
#define Para_1_FLASH_SIZE				(Para_1_FLASH_END_ADDR - Para_1_FLASH_START_ADDR + 1)

#define Para_2_FLASH_START_ADDR			(0X080C0000)
#define Para_2_FLASH_END_ADDR			(0x080E0000-1)
#define Para_2_FLASH_SIZE				(Para_2_FLASH_END_ADDR - Para_2_FLASH_START_ADDR + 1)


#define		Para_1_Update_FLAG_ADDR		(Para_1_FLASH_START_ADDR)
#define		Para_1_Update_FLAG_Set		(0xaaaaaaaa)
#define		Para_1_Update_FLAG_Clear	(0x55555555)

#define 	AppIntegrityAddr          (Para_1_Update_FLAG_ADDR + 4)
#define		AppIntegrity_FLAG_Set			(0xaaaaaaaa)
#define		AppIntegrity_FLAG_Clear		(0x55555555)

#define 	PackReceiveAddr       		(AppIntegrityAddr + 4)
#define		PackReceive_FLAG_Set			(0xaaaaaaaa)
#define		PackReceive_FLAG_Clear		(0x55555555)

typedef struct
{
	uint8_t SetFlag;
	uint8_t RetFlag;
	uint8_t WaitFlag;

	int16_t BeforePower;
	int16_t SetActPower;
	
	int16_t Current;
	uint16_t BatTol_Vol;

	uint32_t rxTick;
	uint32_t txTick;
	uint32_t TickDiff;
	
}tPowerTest;

typedef struct
{
	uint8_t connect;//0:没有任何连接 1:ECS 2:上位机
	
}tLANinfo;
//extern tLANinfo LANinfo;


typedef struct
{
	uint8_t valid;//0-无效 1-有效
	uint8_t rxflag;
	uint32_t cnt;//计时
	
}tPCSPowerSTRinfo;

void AdjustPCSPowerSTR_Timeout(void);

#endif
