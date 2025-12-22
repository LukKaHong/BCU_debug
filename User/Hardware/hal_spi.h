#ifndef _HAL_SPI_H
#define _HAL_SPI_H
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
#include "main.h"

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

typedef enum
{
	SPI_W25Q,
	SPI_ToCAN,
	SPI_NUN
}SPITYPE;

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/
/*W25Q*/
#define		W25Q_CS_PORT			Flash_SPI_CS_GPIO_Port
#define		W25Q_CS_PIN				Flash_SPI_CS_Pin

/*SPI to CAN*/
// #define		SpiToCan_CS_PORT			GPIOA
// #define		SpiToCan_CS_PIN				GPIO_PIN_4

/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/

void SpiCsLow(uint8_t spitype);
void SpiCsHigh(uint8_t spitype);
uint8_t SpiSendByte(uint8_t spitype,uint8_t byte);
uint8_t SpiReadByte(uint8_t spitype);
void SpiWrArray(uint8_t spitype,uint32_t NumByteToWrite, uint8_t *pBuf);
void SpiRdArray(uint8_t spitype,uint32_t NumByteToRead, uint8_t *pBuf);
void SpiWrAndRd(uint8_t spitype,uint8_t *txData, uint8_t txLen, uint8_t *rxData, uint8_t rxLen); 
/*
----------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------
*/



#endif
