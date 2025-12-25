#include "hal_spi.h"
#include "spi.h"


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#define DUMMY_BYTE 0xA5


/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SpiCsLow(uint8_t spitype)
{
	switch(spitype)
	{	
		case SPI_W25Q:
			HAL_GPIO_WritePin(W25Q_CS_PORT,W25Q_CS_PIN,GPIO_PIN_RESET);
			break;
		case SPI_LAN1:
			HAL_GPIO_WritePin(LAN1_CS_PORT,LAN1_CS_PIN,GPIO_PIN_RESET);
			break;
		case SPI_LAN2:
			HAL_GPIO_WritePin(LAN2_CS_PORT,LAN2_CS_PIN,GPIO_PIN_RESET);
			break;
		// case SPI_ToCAN:
		// 	HAL_GPIO_WritePin(SpiToCan_CS_PORT,SpiToCan_CS_PIN,GPIO_PIN_RESET);
		// 	break;	
		default:
			break;
	}
}
 
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SpiCsHigh(uint8_t spitype)
{
	switch(spitype)
	{	
		case SPI_W25Q:
			HAL_GPIO_WritePin(W25Q_CS_PORT,W25Q_CS_PIN,GPIO_PIN_SET);
			break;
		case SPI_LAN1:
			HAL_GPIO_WritePin(LAN1_CS_PORT,LAN1_CS_PIN,GPIO_PIN_SET);
			break;
		case SPI_LAN2:
			HAL_GPIO_WritePin(LAN2_CS_PORT,LAN2_CS_PIN,GPIO_PIN_SET);
			break;
		// case SPI_ToCAN:
		// 	HAL_GPIO_WritePin(SpiToCan_CS_PORT,SpiToCan_CS_PIN,GPIO_PIN_SET);
		// 	break;	
		default:
			break;
	}
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t SpiSendByte(uint8_t spitype,uint8_t byte)
{
	switch(spitype)
	{	
		case SPI_W25Q:
			return SPI_ReadWriteByte(5,byte);
		case SPI_LAN1:
			return SPI_ReadWriteByte(6,byte);
		case SPI_LAN2:
			return SPI_ReadWriteByte(4,byte);
		// case SPI_ToCAN:
		// 	return STM32_SPI_ReadWriteByte(3,byte);		
		default:
			return 0;
	}
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
uint8_t SpiReadByte(uint8_t spitype)
{
	return (SpiSendByte(spitype,DUMMY_BYTE));
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SpiWrArray(uint8_t spitype,uint32_t NumByteToWrite, uint8_t *pBuf)
{	
	while(NumByteToWrite--)
	{
		SpiSendByte(spitype,*pBuf);
		pBuf++;
	}
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SpiRdArray(uint8_t spitype,uint32_t NumByteToRead, uint8_t *pBuf)
{	
	while(NumByteToRead--)
	{
		*pBuf = SpiSendByte(spitype,DUMMY_BYTE);
		pBuf++;
	}
}

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
void SpiWrAndRd(uint8_t spitype,uint8_t *txData, uint8_t txLen, uint8_t *rxData, uint8_t rxLen) 
{
	SpiWrArray(spitype,txLen,txData);
	SpiRdArray(spitype,rxLen,rxData);
}

















