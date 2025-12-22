#ifndef _PCF8523_H
#define _PCF8523_H

#include "main.h"





/*
**************************************************************************************

**************************************************************************************
*/
#define PCF8523_Reg_Ctl_1		(0x00)
#define PCF8523_Reg_Ctl_2		(0x01)
#define PCF8523_Reg_Ctl_3		(0x02)
#define PCF8523_Reg_Sec			(0x03)
#define PCF8523_Reg_Min			(0x04)
#define PCF8523_Reg_Hour		(0x05)
#define PCF8523_Reg_Day			(0x06)
#define PCF8523_Reg_Week		(0x07)
#define PCF8523_Reg_Month		(0x08)
#define PCF8523_Reg_Year		(0x09)
#define PCF8523_Reg_Min_Alm		(0x0A)
#define PCF8523_Reg_Hour_Alm	(0x0B)
#define PCF8523_Reg_Day_Alm		(0x0C)
#define PCF8523_Reg_Week_Alm	(0x0D)
#define PCF8523_Reg_Offset		(0x0E)
#define PCF8523_Reg_Tmr_Clkout	(0x0F)
#define PCF8523_Reg_Tmr_A_Freq	(0x10)
#define PCF8523_Reg_Tmr_A		(0x11)
#define PCF8523_Reg_Tmr_B_Freq	(0x12)
#define PCF8523_Reg_Tmr_B		(0x13)

/*
**************************************************************************************

**************************************************************************************
*/
typedef struct
{
	uint8_t Sec;
	uint8_t Min;
	uint8_t Hour;
	uint8_t Day;
	uint8_t Week;
	uint8_t Month;
	uint8_t Year;
}tSysTime;

typedef struct
{
	uint8_t Ctl_1;
	uint8_t Ctl_2;
	uint8_t Ctl_3;
}tPCF8523_Reg;

typedef enum
{
	PCF8523_CAP_7pf    = 0,
	PCF8523_CAP_12_5pf = 1,
}tPCF8523_CAP;

typedef enum
{
	PCF8523_RTCcir_running = 0,
	PCF8523_RTCcir_frozen  = 1,
}tPCF8523_RTCcir;

typedef enum
{
	PCF8523_HourMode_24 = 0,
	PCF8523_HourMode_12 = 1,
}tPCF8523_HourMode;

typedef enum
{
	PCF8523_SIE_dis = 0,
	PCF8523_SIE_en = 1,
}tPCF8523_SIE;

typedef enum
{
	PCF8523_AIE_dis = 0,
	PCF8523_AIE_en = 1,
}tPCF8523_AIE;

typedef enum
{
	PCF8523_CIE_dis = 0,
	PCF8523_CIE_en = 1,
}tPCF8523_CIE;

typedef enum
{
	PCF8523_WTAF_dis = 0,
	PCF8523_WTAF_en = 1,
}tPCF8523_WTAF;

typedef enum
{
	PCF8523_CTAF_dis = 0,
	PCF8523_CTAF_en = 1,
}tPCF8523_CTAF;

typedef enum
{
	PCF8523_CTBF_dis = 0,
	PCF8523_CTBF_en = 1,
}tPCF8523_CTBF;

typedef enum
{
	PCF8523_SF_dis = 0,
	PCF8523_SF_en = 1,
}tPCF8523_SF;

typedef enum
{
	PCF8523_AF_dis = 0,
	PCF8523_AF_en = 1,
}tPCF8523_AF;

typedef enum
{
	PCF8523_WTAIE_dis = 0,
	PCF8523_WTAIE_en = 1,
}tPCF8523_WTAIE;

typedef enum
{
	PCF8523_CTAIE_dis = 0,
	PCF8523_CTAIE_en = 1,
}tPCF8523_CTAIE;

typedef enum
{
	PCF8523_CTBIE_dis = 0,
	PCF8523_CTBIE_en = 1,
}tPCF8523_CTBIE;

/*
**************************************************************************************

**************************************************************************************
*/
bool PCF8523_Read_Time(tSysTime *time);
bool PCF8523_Write_Time(tSysTime *time);
bool PCF8523_Reset(void);
bool PCF8523_Set_Ctl_1(void);
bool PCF8523_Set_Ctl_2(void);
bool PCF8523_Set_Ctl_3(void);
bool PCF8523_Read_Ctl_1(void);
bool PCF8523_Read_Ctl_2(void);
bool PCF8523_Read_Ctl_3(void);
void PCF8523_Set_Cap(tPCF8523_CAP cap);
void PCF8523_Set_HourMode(tPCF8523_HourMode mode);
void PCF8523_Set_SIE(tPCF8523_SIE sie);
void PCF8523_Set_AIE(tPCF8523_AIE aie);
void PCF8523_Set_CIE(tPCF8523_CIE cie);
void PCF8523_Set_WTAIE(tPCF8523_WTAIE wtaie);
void PCF8523_Set_CTAIE(tPCF8523_CTAIE ctaie);
void PCF8523_Set_CTBIE(tPCF8523_CTBIE ctbie);



#endif
