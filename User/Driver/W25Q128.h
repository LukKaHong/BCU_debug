#ifndef _W25Q128_H
#define _W25Q128_H


#include "main.h"

#define		W25Q_PageSize			256			//每页数据长度
#define		W25Q_SectorSize			4096		//每扇区数据长度
#define		W25Q_BlockSize			65536		//每块数据长度

#define		W25Q_BlockNum			256			//块数量











#define Storage_AlarmRecordStartAddr			(0x00000000)
#define Storage_AlarmRecordEndAddr				(Storage_AlarmRecordStartAddr + AlarmRecordLength * AlarmRecordMaxNum - 1)

#define Storage_RunRecordStartAddr				(Storage_AlarmRecordEndAddr + 1)
#define Storage_RunRecordEndAddr				(Storage_RunRecordStartAddr + RunRecordLength * RunRecordMaxNum - 1)

#define Storage_OperateRecordStartAddr			(Storage_RunRecordEndAddr + 1)
#define Storage_OperateRecordEndAddr			(Storage_OperateRecordStartAddr + OperateRecordLength * OperateRecordMaxNum - 1)







typedef enum
{
	W25Q_FREE,
	W25Q_BUSY,
	W25Q_STATE_NUM
}W25Q_STATE_TYPE;



W25Q_STATE_TYPE W25Q_GetState(void);
void W25Q_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void W25Q_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void W25Q_BlockErase(uint32_t BlockAddr);
void W25Q_SectorErase(uint32_t SectorAddr);
uint8_t W25Q_WriteBytes(uint8_t *buf, uint32_t addr, uint32_t len);
void W25Q_Testing(void);


#endif
