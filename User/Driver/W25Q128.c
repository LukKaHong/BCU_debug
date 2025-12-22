#include "W25Q128.h"
#include "hal_spi.h"




/**********************************************************************************************************************
 *                                                 内部宏                                                             *
 *********************************************************************************************************************/														 													
/* W25Q128 128Mbit = 16MB = 16384KB */
/* 共256块 每块64KB包含16个扇区 每个扇区4KB包含16页 共4096扇区 每页256字节 共0x10000页 */
/* 最大地址0x1000000 */

#define 	WriteEnable               	0x06       	//写使能，设置状态寄存器
#define 	WriteDisable              	0x04       	//写禁止
#define 	ReadStatusRegister        	0x05       	//读状态寄存器
#define 	WriteStatusRegister       	0x01       	//写状态寄存器
#define 	Read_Data                 	0x03       	//读取存储器数据
#define 	FastReadData              	0x0B       	//快速读取存储器数据
#define 	FastReadDualOutput        	0x3B       	//快速双端口输出方式读取存储器数据
#define 	Page_Program              	0x02       	//页面编程--写数据

#define 	BlockErace               	0xD8       	//块擦除
#define 	SectorErace               	0x20       	//扇区擦除
#define 	ChipErace                 	0xC7       	//片擦除
#define 	Power_Down                	0xB9       	//掉电模式

#define 	ReleacePowerDown          	0xAB       	//退出掉电模式
#define 	ReadDeviceID              	0xAB       	//获取设备ID信息

#define 	ReadDeviceID              	0xAB       	//退出掉电模式、设备ID信息
#define 	ReadManuIDDeviceID        	0x90       	//读取制造厂商ID信息和设备ID信息
#define 	ReadJedec_ID              	0x9F       	//JEDEC的ID信息




/**********************************************************************************************************************
 *                                                 局部函数原型                                                       *
 *********************************************************************************************************************/
static void W25Q_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
static void W25Q_WriteEnable(void);
static void W25Q_WaitForWriteEnd(void);

/**********************************************************************************************************************
 *                                                 全局函数实现                                                       *
 *********************************************************************************************************************/
/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_GetState
 * *     >参    数  :void
 * *     >返 回 值  :void
 * *     >描    述  :获取状态
 * *
 * *****************************************************************************/
W25Q_STATE_TYPE W25Q_GetState(void)
{
  uint8_t FLASH_Status = 0;
	
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,ReadStatusRegister);
	FLASH_Status = SpiReadByte(SPI_W25Q);
	SpiCsHigh(SPI_W25Q);
	if(WriteStatusRegister == (FLASH_Status & WriteStatusRegister))
	{
		return W25Q_BUSY;
	}
	else
	{
		return W25Q_FREE;
	}
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_Read
 * *     >参    数  :读取数据指针，读地址，数据长度
 * *     >返 回 值  :void
 * *     >描    述  :读取数据
 * *
 * *****************************************************************************/
void W25Q_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,Read_Data);
	SpiSendByte(SPI_W25Q,(ReadAddr & 0xFF0000) >> 16);
	SpiSendByte(SPI_W25Q,(ReadAddr & 0xFF00) >> 8); 
	SpiSendByte(SPI_W25Q,(ReadAddr & 0xFF));	
	
	SpiRdArray(SPI_W25Q,NumByteToRead,pBuffer);
	SpiCsHigh(SPI_W25Q);	
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_Write
 * *     >参    数  :待写入数据指针，写地址，数据长度
 * *     >返 回 值  :void
 * *     >描    述  :写入数据  整页写
 * *
 * *****************************************************************************/
void W25Q_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	if(WriteAddr%W25Q_PageSize != 0)
	{
		return;
	}
	
	while(1)
	{
		if(NumByteToWrite > W25Q_PageSize)
		{
			W25Q_PageWrite(pBuffer,WriteAddr,W25Q_PageSize);
			pBuffer += W25Q_PageSize;
			WriteAddr += W25Q_PageSize;
			NumByteToWrite -= W25Q_PageSize;
		}
		else
		{
			W25Q_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
			break;
		}
	}
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_BlockErase
 * *     >参    数  :块地址
 * *     >返 回 值  :void
 * *     >描    述  :块擦除
 * *
 * *****************************************************************************/
void W25Q_BlockErase(uint32_t BlockAddr)
{
	W25Q_WriteEnable();	
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,BlockErace);
	SpiSendByte(SPI_W25Q,(BlockAddr & 0xFF0000) >> 16);
	SpiSendByte(SPI_W25Q,(BlockAddr & 0xFF00) >> 8); 
	SpiSendByte(SPI_W25Q,(BlockAddr & 0xFF));	
	SpiCsHigh(SPI_W25Q);
	W25Q_WaitForWriteEnd();
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_SectorErase
 * *     >参    数  :扇区地址
 * *     >返 回 值  :void
 * *     >描    述  :扇区擦除
 * *
 * *****************************************************************************/
void W25Q_SectorErase(uint32_t SectorAddr)
{
	W25Q_WriteEnable();	
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,SectorErace);
	SpiSendByte(SPI_W25Q,(SectorAddr & 0xFF0000) >> 16);
	SpiSendByte(SPI_W25Q,(SectorAddr & 0xFF00) >> 8); 
	SpiSendByte(SPI_W25Q,(SectorAddr & 0xFF));	
	SpiCsHigh(SPI_W25Q);
	W25Q_WaitForWriteEnd();
}

/**********************************************************************************************************************
 *                                                 局部函数实现                                                       *
 *********************************************************************************************************************/
/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_PageWrite
 * *     >参    数  :待写入数据指针，写地址，数据长度
 * *     >返 回 值  :void
 * *     >描    述  :页写入数据
 * *
 * *****************************************************************************/
static void W25Q_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	W25Q_WriteEnable();
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,Page_Program);
	SpiSendByte(SPI_W25Q,(WriteAddr & 0xFF0000) >> 16);
	SpiSendByte(SPI_W25Q,(WriteAddr & 0xFF00) >> 8); 
	SpiSendByte(SPI_W25Q,(WriteAddr & 0xFF));
	
	SpiWrArray(SPI_W25Q,NumByteToWrite,pBuffer);
	SpiCsHigh(SPI_W25Q);
	W25Q_WaitForWriteEnd();
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_WriteEnable
 * *     >参    数  :void
 * *     >返 回 值  :void
 * *     >描    述  :写使能
 * *
 * *****************************************************************************/
static void W25Q_WriteEnable(void)
{
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,WriteEnable);
	SpiCsHigh(SPI_W25Q);
}

/******************************************************************************
 * *
 * *     >编 辑 者  :KK
 * *     >创建时间  :2024年1月22日
 * *     >更 新 者  :
 * *     >更新时间  :
 * *     >函数名称  :W25Q_WaitForWriteEnd
 * *     >参    数  :void
 * *     >返 回 值  :void
 * *     >描    述  :等待写入完成
 * *
 * *****************************************************************************/
static void W25Q_WaitForWriteEnd(void)
{
   uint8_t FLASH_Status = 0;
   uint8_t cnt = 0;
	
	SpiCsLow(SPI_W25Q);
	SpiSendByte(SPI_W25Q,ReadStatusRegister);
	do
	{
		FLASH_Status = SpiReadByte(SPI_W25Q);

		if(++cnt >= 10000)
			break;

	}while(WriteStatusRegister == (FLASH_Status & WriteStatusRegister));
	SpiCsHigh(SPI_W25Q);
}



/*
*********************************************************************************************************
*
*********************************************************************************************************
*/
#define W25Q_START_ADDR (0x00000000)
#define W25Q_END_ADDR (0x01000000)

uint8_t W25Q_BUFF[W25Q_SectorSize];

uint8_t W25Q_WriteBytes(uint8_t *buf, uint32_t addr, uint32_t len)
{
	uint32_t sectorpos;
	uint16_t sectoroff;
	uint16_t sectorremain;
	uint16_t i;
	uint8_t *sectorbuf = W25Q_BUFF;

	addr += W25Q_START_ADDR;
	if(addr + len > W25Q_END_ADDR)
	{
		return 0;
	}

	sectorpos = addr / W25Q_SectorSize;//sector start address
	sectoroff = addr % W25Q_SectorSize;//offset in the sector
	sectorremain = W25Q_SectorSize - sectoroff;//size of free space of sector

	if(len <= sectorremain)
		sectorremain = len;

	while(1)
	{
		W25Q_Read(sectorbuf, sectorpos*W25Q_SectorSize, W25Q_SectorSize);//read data of all sector

		W25Q_SectorErase(addr);//erase sector

		for(i = 0;i < sectorremain;i++)
		{
			sectorbuf[i+sectoroff] = buf[i];//put txdata to buff
		}

		W25Q_Write(sectorbuf, sectorpos*W25Q_SectorSize, W25Q_SectorSize);//write data

		if(len == sectorremain)//finsh
		{
			break;
		}
		else
		{
			sectorpos++;//add address of sector
			sectoroff = 0;//offset set to zero

			buf	 += sectorremain;
			addr += sectorremain;
			len  -= sectorremain;

			if(len > W25Q_SectorSize)
				sectorremain = W25Q_SectorSize;
			else
				sectorremain = len;
		}
	}

	return 1;
}


