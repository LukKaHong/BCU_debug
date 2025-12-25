/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_SPI_BUS_H
#define __BSP_SPI_BUS_H

#include <stdint.h>


/*
*********************************************************************************************************
*	                            时钟，引脚，DMA，中断等宏定义
*********************************************************************************************************


//SPI1  W1
#define SPI1_CLK_ENABLE()			__HAL_RCC_SPI1_CLK_ENABLE()

#define SPI1_FORCE_RESET()			__HAL_RCC_SPI1_FORCE_RESET()
#define SPI1_RELEASE_RESET()		__HAL_RCC_SPI1_RELEASE_RESET()

#define SPI1_SCK_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_SCK_GPIO				GPIOA
#define SPI1_SCK_PIN				GPIO_PIN_5
#define SPI1_SCK_AF					GPIO_AF5_SPI1

#define SPI1_MISO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MISO_GPIO				GPIOA
#define SPI1_MISO_PIN 				GPIO_PIN_6
#define SPI1_MISO_AF				GPIO_AF5_SPI1

#define SPI1_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MOSI_GPIO				GPIOA
#define SPI1_MOSI_PIN 				GPIO_PIN_7
#define SPI1_MOSI_AF				GPIO_AF5_SPI1

#define SPI1_SCS_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_SCS_GPIO				GPIOA
#define SPI1_SCS_PIN				GPIO_PIN_4

#define W1_RESET_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()
#define W1_RESET_GPIO				GPIOC
#define W1_RESET_PIN 				GPIO_PIN_4

#define W1_INT_CLK_ENABLE()			__HAL_RCC_GPIOC_CLK_ENABLE()
#define W1_INT_GPIO					GPIOC
#define W1_INT_PIN 					GPIO_PIN_5

//SPI2  DATA FLASH
#define SPI2_CLK_ENABLE()			__HAL_RCC_SPI2_CLK_ENABLE()

#define SPI2_FORCE_RESET()			__HAL_RCC_SPI2_FORCE_RESET()
#define SPI2_RELEASE_RESET()		__HAL_RCC_SPI2_RELEASE_RESET()

#define SPI2_SCK_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_SCK_GPIO				GPIOB
#define SPI2_SCK_PIN				GPIO_PIN_13
#define SPI2_SCK_AF					GPIO_AF5_SPI2

#define SPI2_MISO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_GPIO				GPIOB
#define SPI2_MISO_PIN 				GPIO_PIN_14
#define SPI2_MISO_AF				GPIO_AF5_SPI2

#define SPI2_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MOSI_GPIO				GPIOB
#define SPI2_MOSI_PIN 				GPIO_PIN_15
#define SPI2_MOSI_AF				GPIO_AF5_SPI2

#define SPI2_SCS_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_SCS_GPIO				GPIOB
#define SPI2_SCS_PIN 				GPIO_PIN_12

//SPI3  Flash
#define SPI3_CLK_ENABLE()			__HAL_RCC_SPI3_CLK_ENABLE()

#define DMAx_CLK_ENABLE()			__HAL_RCC_DMA1_CLK_ENABLE()

#define SPI3_FORCE_RESET()			__HAL_RCC_SPI3_FORCE_RESET()
#define SPI3_RELEASE_RESET()		__HAL_RCC_SPI3_RELEASE_RESET()

#define SPI3_SCK_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI3_SCK_GPIO				GPIOB
#define SPI3_SCK_PIN				GPIO_PIN_3
#define SPI3_SCK_AF					GPIO_AF6_SPI3

#define SPI3_MISO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI3_MISO_GPIO				GPIOB
#define SPI3_MISO_PIN 				GPIO_PIN_4
#define SPI3_MISO_AF				GPIO_AF6_SPI3

#define SPI3_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI3_MOSI_GPIO				GPIOB
#define SPI3_MOSI_PIN 				GPIO_PIN_5
#define SPI3_MOSI_AF				GPIO_AF7_SPI3

//SPI4  W1
#define SPI4_CLK_ENABLE()			__HAL_RCC_SPI4_CLK_ENABLE()

#define SPI4_FORCE_RESET()			__HAL_RCC_SPI4_FORCE_RESET()
#define SPI4_RELEASE_RESET()		__HAL_RCC_SPI4_RELEASE_RESET()

#define SPI4_SCK_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_SCK_GPIO				GPIOE
#define SPI4_SCK_PIN				GPIO_PIN_2
#define SPI4_SCK_AF					GPIO_AF5_SPI4

#define SPI4_MISO_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_MISO_GPIO				GPIOE
#define SPI4_MISO_PIN 				GPIO_PIN_5
#define SPI4_MISO_AF				GPIO_AF5_SPI4

#define SPI4_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_MOSI_GPIO				GPIOE
#define SPI4_MOSI_PIN 				GPIO_PIN_6
#define SPI4_MOSI_AF				GPIO_AF5_SPI4

#define SPI4_SCS_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define SPI4_SCS_GPIO				GPIOE
#define SPI4_SCS_PIN				GPIO_PIN_4

#define SPI5_CLK_ENABLE()			__HAL_RCC_SPI5_CLK_ENABLE()

#define SPI5_SCK_CLK_ENABLE()		__HAL_RCC_GPIOH_CLK_ENABLE()
#define SPI5_SCK_GPIO				GPIOH
#define SPI5_SCK_PIN				GPIO_PIN_6
#define SPI5_SCK_AF					GPIO_AF5_SPI5

#define SPI5_MISO_CLK_ENABLE()		__HAL_RCC_GPIOH_CLK_ENABLE()
#define SPI5_MISO_GPIO				GPIOH
#define SPI5_MISO_PIN 				GPIO_PIN_7
#define SPI5_MISO_AF				GPIO_AF5_SPI5

#define SPI5_MOSI_CLK_ENABLE()		__HAL_RCC_GPIOF_CLK_ENABLE()
#define SPI5_MOSI_GPIO				GPIOF
#define SPI5_MOSI_PIN 				GPIO_PIN_11
#define SPI5_MOSI_AF				GPIO_AF5_SPI5

#define SPI5_SCS_CLK_ENABLE()		__HAL_RCC_GPIOH_CLK_ENABLE()
#define SPI5_SCS_GPIO				GPIOH
#define SPI5_SCS_PIN				GPIO_PIN_5

#define W2_RESET_CLK_ENABLE()		__HAL_RCC_GPIOH_CLK_ENABLE()
#define W2_RESET_GPIO				GPIOH
#define W2_RESET_PIN 				GPIO_PIN_9

#define W2_INT_CLK_ENABLE()			__HAL_RCC_GPIOH_CLK_ENABLE()
#define W2_INT_GPIO					GPIOH
#define W2_INT_PIN 					GPIO_PIN_8

//SPI6  AD
#define SPI6_CLK_ENABLE()			__HAL_RCC_SPI6_CLK_ENABLE()

#define SPI6_FORCE_RESET()			__HAL_RCC_SPI6_FORCE_RESET()
#define SPI6_RELEASE_RESET()		__HAL_RCC_SPI6_RELEASE_RESET()

#define SPI6_SCK_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI6_SCK_GPIO				GPIOB
#define SPI6_SCK_PIN				GPIO_PIN_3
#define SPI6_SCK_AF					GPIO_AF8_SPI6

#define SPI6_MISO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI6_MISO_GPIO				GPIOB
#define SPI6_MISO_PIN 				GPIO_PIN_4
#define SPI6_MISO_AF				GPIO_AF8_SPI6*/



/* 重定义下SPI SCK时钟，方便移植 */
#define SPI_BAUDRATEPRESCALER_100M      SPI_BAUDRATEPRESCALER_2			/* 100M */
#define SPI_BAUDRATEPRESCALER_50M       SPI_BAUDRATEPRESCALER_4			/* 50M */
#define SPI_BAUDRATEPRESCALER_12_5M     SPI_BAUDRATEPRESCALER_8			/* 12.5M */
#define SPI_BAUDRATEPRESCALER_6_25M     SPI_BAUDRATEPRESCALER_16		/* 6.25M */
#define SPI_BAUDRATEPRESCALER_3_125M    SPI_BAUDRATEPRESCALER_32		/* 3.125M */
#define SPI_BAUDRATEPRESCALER_1_5625M   SPI_BAUDRATEPRESCALER_64		/* 1.5625M */
#define SPI_BAUDRATEPRESCALER_781_25K   SPI_BAUDRATEPRESCALER_128		/* 781.25K */
#define SPI_BAUDRATEPRESCALER_390_625K  SPI_BAUDRATEPRESCALER_256		/* 390.625K */


#define	SPI_BUFFER_SIZE		(512)				/*  */


void bsp_InitSPIBus(void);

void bsp_InitSPI1Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
void bsp_InitSPI2Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
void bsp_InitSPI3Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
void bsp_InitSPI4Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
void bsp_InitSPI5Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);
void bsp_InitSPI6Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity);

//SPIFlash
void bsp_spiflashTransfer(void);

void bsp_SpiFlashBusEnter(void);
void bsp_SpiFlashBusExit(void);
uint8_t bsp_SpiFlashBusBusy(void);

extern uint8_t g_spiFlashTxBuf[SPI_BUFFER_SIZE];
extern uint8_t g_spiFlashRxBuf[SPI_BUFFER_SIZE];
extern uint32_t g_spiFlashLen;
extern uint8_t g_spiflash_busy;

//AD
extern uint32_t g_spiADLen;	
extern uint8_t g_spiADTxBuf[SPI_BUFFER_SIZE];  
extern uint8_t g_spiADRxBuf[SPI_BUFFER_SIZE];
void bsp_spiADTransfer(uint16_t Size);

//w5500
uint8_t bsp_spiw55001TransferByte(uint8_t byte);
uint8_t bsp_spiw55002TransferByte(uint8_t byte);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
