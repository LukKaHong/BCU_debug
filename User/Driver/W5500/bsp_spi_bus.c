/*
*********************************************************************************************************
*
*	模块名称 : SPI总线驱动
*	文件名称 : bsp_spi_bus.c
*	版    本 : V1.3
*	说    明 : SPI总线底层驱动。提供SPI配置、收发数据、多设备共享SPI功能.
*	修改记录 :
*		版本号  日期        作者    说明
*		V1.0	2020-03-14  wr    适配STM32H7。
*
*	
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "main.h"
#include "hal_spi.h"
#include "spi.h"
/*
*********************************************************************************************************
*	                             选择DMA，中断或者查询方式
*********************************************************************************************************
*/
//#define USE_SPI_DMA    /* DMA方式  */
//#define USE_SPI_INT    /* 中断方式 */
#define USE_SPI_POLL   /* 查询方式 */



enum {
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};


/*
*********************************************************************************************************
*	                                           变量
*********************************************************************************************************
*/
static SPI_HandleTypeDef hspi1;
static SPI_HandleTypeDef hspi2;
static SPI_HandleTypeDef hspi3;
static SPI_HandleTypeDef hspi4;
static SPI_HandleTypeDef hspi5;
static SPI_HandleTypeDef hspi6;

static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;

//flash
uint32_t g_spiFlashLen;	
uint8_t  g_spiflash_busy; /* SPI忙状态，0表示不忙，1表示忙 */
__IO uint32_t wTransferflashState = TRANSFER_WAIT;

uint8_t g_spiFlashTxBuf[SPI_BUFFER_SIZE];  
uint8_t g_spiFlashRxBuf[SPI_BUFFER_SIZE];

//AD
uint32_t g_spiADLen;	
uint8_t g_spiADTxBuf[SPI_BUFFER_SIZE];  
uint8_t g_spiADRxBuf[SPI_BUFFER_SIZE];

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPIBus
*	功能说明: 配置SPI总线。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{	
	g_spiflash_busy = 0;
	
	//DATA FLash
	bsp_InitSPI2Param(SPI_BAUDRATEPRESCALER_8, SPI_PHASE_1EDGE, SPI_POLARITY_LOW);
	
	//AD 时钟频率200Mhz/SPI_BAUDRATEPRESCALER_4
	bsp_InitSPI6Param(SPI_BAUDRATEPRESCALER_4, SPI_PHASE_2EDGE, SPI_POLARITY_HIGH);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI2Param
*	功能说明: 配置SPI总线参数，时钟分频，时钟相位和时钟极性。
*	形    参: _BaudRatePrescaler  SPI总线时钟分频设置，支持的参数如下：
*                                 SPI_BAUDRATEPRESCALER_2    2分频
*                                 SPI_BAUDRATEPRESCALER_4    4分频
*                                 SPI_BAUDRATEPRESCALER_8    8分频
*                                 SPI_BAUDRATEPRESCALER_16   16分频
*                                 SPI_BAUDRATEPRESCALER_32   32分频
*                                 SPI_BAUDRATEPRESCALER_64   64分频
*                                 SPI_BAUDRATEPRESCALER_128  128分频
*                                 SPI_BAUDRATEPRESCALER_256  256分频
*                                                        
*             _CLKPhase           时钟相位，支持的参数如下：
*                                 SPI_PHASE_1EDGE     SCK引脚的第1个边沿捕获传输的第1个数据
*                                 SPI_PHASE_2EDGE     SCK引脚的第2个边沿捕获传输的第1个数据
*                                 
*             _CLKPolarity        时钟极性，支持的参数如下：
*                                 SPI_POLARITY_LOW    SCK引脚在空闲状态处于低电平
*                                 SPI_POLARITY_HIGH   SCK引脚在空闲状态处于高电平
*
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI1Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{

	/* 设置SPI参数 */
	hspi1.Instance               = SPI1;                   /* 例化SPI */
	hspi1.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi1.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi1.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi1.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi1.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi1.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi1.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi1.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi1.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi1.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */

	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi1) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI2Param
*	功能说明: 配置SPI总线参数，时钟分频，时钟相位和时钟极性。
*	形    参: _BaudRatePrescaler  SPI总线时钟分频设置，支持的参数如下：
*                                 SPI_BAUDRATEPRESCALER_2    2分频
*                                 SPI_BAUDRATEPRESCALER_4    4分频
*                                 SPI_BAUDRATEPRESCALER_8    8分频
*                                 SPI_BAUDRATEPRESCALER_16   16分频
*                                 SPI_BAUDRATEPRESCALER_32   32分频
*                                 SPI_BAUDRATEPRESCALER_64   64分频
*                                 SPI_BAUDRATEPRESCALER_128  128分频
*                                 SPI_BAUDRATEPRESCALER_256  256分频
*                                                        
*             _CLKPhase           时钟相位，支持的参数如下：
*                                 SPI_PHASE_1EDGE     SCK引脚的第1个边沿捕获传输的第1个数据
*                                 SPI_PHASE_2EDGE     SCK引脚的第2个边沿捕获传输的第1个数据
*                                 
*             _CLKPolarity        时钟极性，支持的参数如下：
*                                 SPI_POLARITY_LOW    SCK引脚在空闲状态处于低电平
*                                 SPI_POLARITY_HIGH   SCK引脚在空闲状态处于高电平
*
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI2Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{

	/* 设置SPI参数 */
	hspi2.Instance               = SPI2;                   /* 例化SPI */
	hspi2.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi2.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi2.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi2.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi2.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi2.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi2.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi2.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi2.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi2.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi2.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi2.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */

	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi2) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}	
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI3Param
*	功能说明: 配置SPI3总线参数，时钟分频，时钟相位和时钟极性。
*	形    参: _BaudRatePrescaler  SPI总线时钟分频设置，支持的参数如下：
*                                 SPI_BAUDRATEPRESCALER_2    2分频
*                                 SPI_BAUDRATEPRESCALER_4    4分频
*                                 SPI_BAUDRATEPRESCALER_8    8分频
*                                 SPI_BAUDRATEPRESCALER_16   16分频
*                                 SPI_BAUDRATEPRESCALER_32   32分频
*                                 SPI_BAUDRATEPRESCALER_64   64分频
*                                 SPI_BAUDRATEPRESCALER_128  128分频
*                                 SPI_BAUDRATEPRESCALER_256  256分频
*                                                        
*             _CLKPhase           时钟相位，支持的参数如下：
*                                 SPI_PHASE_1EDGE     SCK引脚的第1个边沿捕获传输的第1个数据
*                                 SPI_PHASE_2EDGE     SCK引脚的第2个边沿捕获传输的第1个数据
*                                 
*             _CLKPolarity        时钟极性，支持的参数如下：
*                                 SPI_POLARITY_LOW    SCK引脚在空闲状态处于低电平
*                                 SPI_POLARITY_HIGH   SCK引脚在空闲状态处于高电平
*
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI3Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{
	/* 设置SPI参数 */
	hspi3.Instance               = SPI3;                   /* 例化SPI */
	hspi3.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi3.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi3.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi3.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi3.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi3.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi3.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi3.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi3.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi3.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi3.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi3.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi3.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi3.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */
	
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi3) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI4Param
*	功能说明: 配置SPI4总线参数，时钟分频，时钟相位和时钟极性。
*	形    参: _BaudRatePrescaler  SPI总线时钟分频设置，支持的参数如下：
*                                 SPI_BAUDRATEPRESCALER_2    2分频
*                                 SPI_BAUDRATEPRESCALER_4    4分频
*                                 SPI_BAUDRATEPRESCALER_8    8分频
*                                 SPI_BAUDRATEPRESCALER_16   16分频
*                                 SPI_BAUDRATEPRESCALER_32   32分频
*                                 SPI_BAUDRATEPRESCALER_64   64分频
*                                 SPI_BAUDRATEPRESCALER_128  128分频
*                                 SPI_BAUDRATEPRESCALER_256  256分频
*                                                        
*             _CLKPhase           时钟相位，支持的参数如下：
*                                 SPI_PHASE_1EDGE     SCK引脚的第1个边沿捕获传输的第1个数据
*                                 SPI_PHASE_2EDGE     SCK引脚的第2个边沿捕获传输的第1个数据
*                                 
*             _CLKPolarity        时钟极性，支持的参数如下：
*                                 SPI_POLARITY_LOW    SCK引脚在空闲状态处于低电平
*                                 SPI_POLARITY_HIGH   SCK引脚在空闲状态处于高电平
*
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI4Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{
	/* 设置SPI参数 */
	hspi4.Instance               = SPI4;                   /* 例化SPI */
	hspi4.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi4.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi4.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi4.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi4.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi4.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi4.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi4.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi4.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi4.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi4.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi4.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi4.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi4.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */
	
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi4) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi4) != HAL_OK)
	{
		Error_Handler();
	}	
}

void bsp_InitSPI5Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{
	/* 设置SPI参数 */
	hspi5.Instance               = SPI5;                   /* 例化SPI */
	hspi5.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi5.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi5.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi5.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi5.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi5.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi5.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi5.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi5.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi5.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi5.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi5.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi5.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi5.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */
	
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi5) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi5) != HAL_OK)
	{
		Error_Handler();
	}	
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPI6Param
*	功能说明: 配置SPI4总线参数，时钟分频，时钟相位和时钟极性。
*	形    参: _BaudRatePrescaler  SPI总线时钟分频设置，支持的参数如下：
*                                 SPI_BAUDRATEPRESCALER_2    2分频
*                                 SPI_BAUDRATEPRESCALER_4    4分频
*                                 SPI_BAUDRATEPRESCALER_8    8分频
*                                 SPI_BAUDRATEPRESCALER_16   16分频
*                                 SPI_BAUDRATEPRESCALER_32   32分频
*                                 SPI_BAUDRATEPRESCALER_64   64分频
*                                 SPI_BAUDRATEPRESCALER_128  128分频
*                                 SPI_BAUDRATEPRESCALER_256  256分频
*                                                        
*             _CLKPhase           时钟相位，支持的参数如下：
*                                 SPI_PHASE_1EDGE     SCK引脚的第1个边沿捕获传输的第1个数据
*                                 SPI_PHASE_2EDGE     SCK引脚的第2个边沿捕获传输的第1个数据
*                                 
*             _CLKPolarity        时钟极性，支持的参数如下：
*                                 SPI_POLARITY_LOW    SCK引脚在空闲状态处于低电平
*                                 SPI_POLARITY_HIGH   SCK引脚在空闲状态处于高电平
*
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitSPI6Param(uint32_t _BaudRatePrescaler, uint32_t _CLKPhase, uint32_t _CLKPolarity)
{
	/* 设置SPI参数 */
	hspi6.Instance               = SPI6;                   /* 例化SPI */
	hspi6.Init.BaudRatePrescaler = _BaudRatePrescaler;     /* 设置波特率 */
	hspi6.Init.Direction         = SPI_DIRECTION_2LINES;   /* 全双工 */
	hspi6.Init.CLKPhase          = _CLKPhase;              /* 配置时钟相位 */
	hspi6.Init.CLKPolarity       = _CLKPolarity;           /* 配置时钟极性 */
	hspi6.Init.DataSize          = SPI_DATASIZE_8BIT;      /* 设置数据宽度 */
	hspi6.Init.FirstBit          = SPI_FIRSTBIT_MSB;       /* 数据传输先传高位 */
	hspi6.Init.TIMode            = SPI_TIMODE_DISABLE;     /* 禁止TI模式  */
	hspi6.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE; /* 禁止CRC */
	hspi6.Init.CRCPolynomial     = 7;                       /* 禁止CRC后，此位无效 */
	hspi6.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;     /* 禁止CRC后，此位无效 */
	hspi6.Init.NSS               = SPI_NSS_SOFT;               /* 使用软件方式管理片选引脚 */
	hspi6.Init.FifoThreshold     = SPI_FIFO_THRESHOLD_01DATA;  /* 设置FIFO大小是一个数据项 */
	hspi6.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;      /* 禁止脉冲输出 */
	hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; /* 禁止SPI后，SPI相关引脚保持当前状态 */  
	hspi6.Init.Mode 			 = SPI_MODE_MASTER;            /* SPI工作在主控模式 */
	
	/* 复位SPI */
	if(HAL_SPI_DeInit(&hspi6) != HAL_OK)
	{
		Error_Handler();     
	}

	/* 初始化SPI */
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		Error_Handler();
	}	
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitSPIParam
*	功能说明: 配置SPI总线时钟，GPIO，中断，DMA等
*	形    参: SPI_HandleTypeDef 类型指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
//void HAL_SPI_MspInit(SPI_HandleTypeDef *_hspi)
//{
	/* 配置 SPI总线GPIO : SCK MOSI MISO */
//	{
//		GPIO_InitTypeDef  GPIO_InitStruct = {0};
//  
//		if(_hspi->Instance==SPI1)
//		{
//			/* SPI和GPIP时钟 */
//			SPI1_SCK_CLK_ENABLE();
//			SPI1_MISO_CLK_ENABLE();
//			SPI1_MOSI_CLK_ENABLE();
//			SPI1_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI1_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI1_SCK_AF;
//			HAL_GPIO_Init(SPI1_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI1_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI1_MISO_AF;
//			HAL_GPIO_Init(SPI1_MISO_GPIO, &GPIO_InitStruct);

//			/* SPI MOSI */
//			GPIO_InitStruct.Pin = SPI1_MOSI_PIN;
//			GPIO_InitStruct.Alternate = SPI1_MOSI_AF;
//			HAL_GPIO_Init(SPI1_MOSI_GPIO, &GPIO_InitStruct);
//			
//			/* SPI SCS */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin		  = SPI1_SCS_PIN;
//			HAL_GPIO_Init(SPI1_SCS_GPIO, &GPIO_InitStruct);
//			
//			/* W1 INT */
//			GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W1_INT_PIN;
//			HAL_GPIO_Init(W1_INT_GPIO, &GPIO_InitStruct);
//			
//			/* W1 RESET */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W1_RESET_PIN;
//			HAL_GPIO_Init(W1_RESET_GPIO, &GPIO_InitStruct);
//		
//		}
//		
//		if(_hspi->Instance==SPI2)
//		{
//			/* SPI和GPIP时钟 */
//			SPI2_SCK_CLK_ENABLE();
//			SPI2_MISO_CLK_ENABLE();
//			SPI2_MOSI_CLK_ENABLE();
//			SPI2_SCS_CLK_ENABLE();			
//			SPI2_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI2_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI2_SCK_AF;
//			HAL_GPIO_Init(SPI2_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI2_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI2_MISO_AF;
//			HAL_GPIO_Init(SPI2_MISO_GPIO, &GPIO_InitStruct);

//			/* SPI MOSI */
//			GPIO_InitStruct.Pin = SPI2_MOSI_PIN;
//			GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
//			HAL_GPIO_Init(SPI2_MOSI_GPIO, &GPIO_InitStruct);
//		}
//		
//		if(_hspi->Instance==SPI3)
//		{
//			/* SPI和GPIP时钟 */
//			SPI3_SCK_CLK_ENABLE();
//			SPI3_MISO_CLK_ENABLE();
//			SPI3_MOSI_CLK_ENABLE();
//			SPI3_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI3_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI3_SCK_AF;
//			HAL_GPIO_Init(SPI3_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI3_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI3_MISO_AF;
//			HAL_GPIO_Init(SPI3_MISO_GPIO, &GPIO_InitStruct);

//			/* SPI MOSI */
//			GPIO_InitStruct.Pin = SPI3_MOSI_PIN;
//			GPIO_InitStruct.Alternate = SPI3_MOSI_AF;
//			HAL_GPIO_Init(SPI3_MOSI_GPIO, &GPIO_InitStruct);
//		}
//		
//		if(_hspi->Instance==SPI4)
//		{
//			/* SPI和GPIP时钟 */
//			SPI4_SCK_CLK_ENABLE();
//			SPI4_MISO_CLK_ENABLE();
//			SPI4_MOSI_CLK_ENABLE();
//			SPI4_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI4_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI4_SCK_AF;
//			HAL_GPIO_Init(SPI4_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI4_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI4_MISO_AF;
//			HAL_GPIO_Init(SPI4_MISO_GPIO, &GPIO_InitStruct);

//			/* SPI MOSI */
//			GPIO_InitStruct.Pin = SPI4_MOSI_PIN;
//			GPIO_InitStruct.Alternate = SPI4_MOSI_AF;
//			HAL_GPIO_Init(SPI4_MOSI_GPIO, &GPIO_InitStruct);
//			
//			/* SPI SCS */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin		  = SPI4_SCS_PIN;
//			HAL_GPIO_Init(SPI4_SCS_GPIO, &GPIO_InitStruct);
//			
//			/* W2 INT */
//			GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W2_INT_PIN;
//			HAL_GPIO_Init(W2_INT_GPIO, &GPIO_InitStruct);
//			
//			/* W2 RESET */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W2_RESET_PIN;
//			HAL_GPIO_Init(W2_RESET_GPIO, &GPIO_InitStruct);
//		
//		}
//		
//		if(_hspi->Instance==SPI5)
//		{
//			/* SPI和GPIP时钟 */
//			SPI5_SCK_CLK_ENABLE();
//			SPI5_MISO_CLK_ENABLE();
//			SPI5_MOSI_CLK_ENABLE();
//			SPI5_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI5_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI5_SCK_AF;
//			HAL_GPIO_Init(SPI5_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI5_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI5_MISO_AF;
//			HAL_GPIO_Init(SPI5_MISO_GPIO, &GPIO_InitStruct);

//			/* SPI MOSI */
//			GPIO_InitStruct.Pin = SPI5_MOSI_PIN;
//			GPIO_InitStruct.Alternate = SPI5_MOSI_AF;
//			HAL_GPIO_Init(SPI5_MOSI_GPIO, &GPIO_InitStruct);
//			
//			/* SPI SCS */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin		  = SPI5_SCS_PIN;
//			HAL_GPIO_Init(SPI5_SCS_GPIO, &GPIO_InitStruct);
//			
//			/* W2 INT */
//			GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W2_INT_PIN;
//			HAL_GPIO_Init(W2_INT_GPIO, &GPIO_InitStruct);
//			
//			/* W2 RESET */
//			GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Pin 	  = W2_RESET_PIN;
//			HAL_GPIO_Init(W2_RESET_GPIO, &GPIO_InitStruct);
//		
//		}		
//		
//		if(_hspi->Instance==SPI6)
//		{
//			/* SPI和GPIP时钟 */
//			SPI6_SCK_CLK_ENABLE();
//			SPI6_MISO_CLK_ENABLE();		
//			SPI6_CLK_ENABLE();

//			/* SPI SCK */
//			GPIO_InitStruct.Pin       = SPI6_SCK_PIN;
//			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//			GPIO_InitStruct.Pull      = GPIO_NOPULL;
//			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//			GPIO_InitStruct.Alternate = SPI6_SCK_AF;
//			HAL_GPIO_Init(SPI6_SCK_GPIO, &GPIO_InitStruct);

//			/* SPI MISO */
//			GPIO_InitStruct.Pin = SPI6_MISO_PIN;
//			GPIO_InitStruct.Alternate = SPI6_MISO_AF;
//			HAL_GPIO_Init(SPI6_MISO_GPIO, &GPIO_InitStruct);
//		}
//		
//	}

//	/* 配置DMA和NVIC */
//	#ifdef USE_SPI_DMA
//	{
//		/* 使能DMA时钟 */
//		DMAx_CLK_ENABLE();      

//		/* SPI DMA发送配置 */		
//		hdma_tx.Instance                 = SPIx_TX_DMA_STREAM;      /* 例化使用的DMA数据流 */
//		hdma_tx.Init.Channel             = SPIx_TX_DMA_CHANNEL;     /* DMA通道配置 */
//		hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;    /* 禁止FIFO*/
//		hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL; /* 禁止FIFO此位不起作用，用于设置阀值 */
//		hdma_tx.Init.MemBurst            = DMA_MBURST_SINGLE;	    /* 禁止FIFO此位不起作用，用于存储器突发 */
//		hdma_tx.Init.PeriphBurst         = DMA_PBURST_SINGLE;	    /* 禁止FIFO此位不起作用，用于外设突发 */
//		hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;    /* 传输方向是从存储器到外设 */  
//		hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;        /* 外设地址自增禁止 */ 
//		hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;         /* 存储器地址自增使能 */  
//		hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;     /* 外设数据传输位宽选择字节，即8bit */ 
//		hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;     /* 存储器数据传输位宽选择字节，即8bit */    
//		hdma_tx.Init.Mode                = DMA_NORMAL;              /* 正常模式 */
//		hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;        /* 优先级低 */
//		
//		/* 复位DMA */
//		if(HAL_DMA_DeInit(&hdma_tx) != HAL_OK)
//		{
//			Error_Handler();     
//		}
//		
//		 /* 初始化DMA */
//		if(HAL_DMA_Init(&hdma_tx) != HAL_OK)
//		{
//			Error_Handler();     
//		}

//		/* 关联DMA句柄到SPI */
//		__HAL_LINKDMA(_hspi, hdmatx, hdma_tx);	

//		/* SPI DMA接收配置 */	
//		hdma_rx.Instance                 = SPIx_RX_DMA_STREAM;     /* 例化使用的DMA数据流 */
//		hdma_rx.Init.Channel             = SPIx_RX_DMA_CHANNEL;    /* DMA通道配置 */
//		hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;   /* 禁止FIFO*/
//		hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;/* 禁止FIFO此位不起作用，用于设置阀值 */
//		hdma_rx.Init.MemBurst            = DMA_MBURST_SINGLE;	   /* 禁止FIFO此位不起作用，用于存储器突发 */
//		hdma_rx.Init.PeriphBurst         = DMA_PBURST_SINGLE;	   /* 禁止FIFO此位不起作用，用于外设突发 */
//		hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;   /* 传输方向从外设到存储器 */  
//		hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;       /* 外设地址自增禁止 */   
//		hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;        /* 存储器地址自增使能 */   
//		hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    /* 外设数据传输位宽选择字节，即8bit */ 
//		hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;    /* 存储器数据传输位宽选择字节，即8bit */   
//		hdma_rx.Init.Mode                = DMA_NORMAL;             /* 正常模式 */
//		hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;      /* 优先级低 */

//		/* 复位DMA */
//		if(HAL_DMA_DeInit(&hdma_rx) != HAL_OK)
//		{
//			Error_Handler();     
//		}
//		
//		 /* 初始化DMA */
//		if(HAL_DMA_Init(&hdma_rx) != HAL_OK)
//		{
//			Error_Handler();     
//		}

//		/* 关联DMA句柄到SPI */
//	   __HAL_LINKDMA(_hspi, hdmarx, hdma_rx);	

//		/* 配置DMA发送中断 */
//		HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);
//		
//		/* 配置DMA接收中断 */
//		HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);
//		
//		/* 配置SPI中断 */
//		HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(SPIx_IRQn);
//	}
//	#endif
//	
//	#ifdef USE_SPI_INT
//		/* 配置SPI中断优先级并使能中断 */
//		HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
//		HAL_NVIC_EnableIRQ(SPIx_IRQn);
//	#endif
//}
	
/*
*********************************************************************************************************
*	函 数 名: bsp_spiw55001TransferByte
*	功能说明: w5500发送1个字节
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiw55001TransferByte(uint8_t byte)
{
	uint8_t d_read;
	uint8_t d_send = byte;

	d_read = SpiSendByte(SPI_LAN2, d_send);
	return d_read;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiw55002TransferByte
*	功能说明: w5500发送1个字节
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_spiw55002TransferByte(uint8_t byte)
{
	uint8_t d_read;
	uint8_t d_send = byte;

	d_read = SpiSendByte(SPI_LAN1, d_send);
	return d_read;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_spiflashTransfer
*	功能说明: 启动数据传输
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiflashTransfer(void)
{
	if (g_spiFlashLen > SPI_BUFFER_SIZE)
	{
		return;
	}
	
	/* DMA方式传输 */
#ifdef USE_SPI_DMA
	wTransferflashState = TRANSFER_WAIT;
	
if(HAL_SPI_TransmitReceive_DMA(&hspi3, (uint8_t*)g_spiFlashTxBuf, (uint8_t *)g_spiFlashRxBuf, g_spiFlashLen) != HAL_OK)	
{
	Error_Handler();
}
	
	while (wTransferflashState == TRANSFER_WAIT)
	{
		;
	}
#endif

	/* 中断方式传输 */	
#ifdef USE_SPI_INT
	wTransferflashState = TRANSFER_WAIT;

if(HAL_SPI_TransmitReceive_IT(&hspi3, (uint8_t*)g_spiFlashTxBuf, (uint8_t *)g_spiFlashRxBuf, g_spiFlashLen) != HAL_OK)	
{
	Error_Handler();
}
	
	while (wTransferflashState == TRANSFER_WAIT)
	{
		;
	}
#endif

	/* 查询方式传输 */	
#ifdef USE_SPI_POLL
	if(HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)g_spiFlashTxBuf, (uint8_t *)g_spiFlashRxBuf, g_spiFlashLen, 1000000) != HAL_OK)	
	{
		Error_Handler();
	}	
#endif
}

/*
*********************************************************************************************************
*	函 数 名: HAL_SPI_TxRxCpltCallback，HAL_SPI_ErrorCallback
*	功能说明: SPI数据传输完成回调和传输错误回调
*	形    参: SPI_HandleTypeDef 类型指针变量
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance==SPI2)
	{
		wTransferflashState = TRANSFER_COMPLETE;
	}
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance==SPI2)
	{
		wTransferflashState = TRANSFER_ERROR;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiFlashBusEnter
*	功能说明: 占用SPI总线
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
void bsp_SpiFlashBusEnter(void)
{
	g_spiflash_busy = 1;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiBusExit
*	功能说明: 释放占用的SPI总线
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
void bsp_SpiFlashBusExit(void)
{
	g_spiflash_busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SpiFlashBusBusy
*	功能说明: 判断SPI总线忙，方法是检测其他SPI芯片的片选信号是否为1
*	形    参: 无
*	返 回 值: 0 表示不忙  1表示忙
*********************************************************************************************************
*/
uint8_t bsp_SpiFlashBusBusy(void)
{
	return g_spiflash_busy;
}

/*
*********************************************************************************************************
*	函 数 名: SPIx_IRQHandler，SPIx_DMA_RX_IRQHandler，SPIx_DMA_TX_IRQHandler
*	功能说明: 中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#ifdef USE_SPI_INT
	void SPIx_IRQHandler(void)
	{
		HAL_SPI_IRQHandler(&hspi);
	}	
#endif

#ifdef USE_SPI_DMA
	void SPIx_DMA_RX_IRQHandler(void)
	{
		HAL_DMA_IRQHandler(hspi.hdmarx);
	}

	void SPIx_DMA_TX_IRQHandler(void)
	{
		HAL_DMA_IRQHandler(hspi.hdmatx);
	}
	
	void SPIx_IRQHandler(void)
	{
		HAL_SPI_IRQHandler(&hspi);
	}	
#endif
	
	
/*
*********************************************************************************************************
*	函 数 名: bsp_spiADTransfer
*	功能说明: 启动数据传输
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_spiADTransfer(uint16_t Size)
{
	if (Size > SPI_BUFFER_SIZE)
	{
		return;
	}
	
	/* DMA方式传输 */
#ifdef USE_SPI_DMA
	wTransferflashState = TRANSFER_WAIT;
	
if(HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)g_spiADTxBuf, (uint8_t *)g_spiADRxBuf, g_spiADLen) != HAL_OK)	
{
	Error_Handler();
}
	
	while (wTransferADState == TRANSFER_WAIT)
	{
		;
	}
#endif

	/* 中断方式传输 */	
#ifdef USE_SPI_INT
	wTransferflashState = TRANSFER_WAIT;

if(HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t*)g_spiADTxBuf, (uint8_t *)g_spiADRxBuf, g_spiADLen) != HAL_OK)	
{
	Error_Handler();
}
	
	while (wTransferADState == TRANSFER_WAIT)
	{
		;
	}
#endif

	/* 查询方式传输 */	
#ifdef USE_SPI_POLL
	if(HAL_SPI_TransmitReceive(&hspi6, (uint8_t*)g_spiADTxBuf, (uint8_t *)g_spiADRxBuf, Size, 1000000) != HAL_OK)	
	{
		Error_Handler();
	}	
#endif
}

/*******************************************************************************************************************/
