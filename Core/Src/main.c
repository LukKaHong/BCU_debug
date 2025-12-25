/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "rng.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ProtocolConvert.h"
#include "SysManage.h"
#include "LAN_ZVPP_ECSUpper_IF.h"
#include "dhcp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/

static uint32_t fac_us=0;							//us?車那㊣㊣?3?那y

#if SYSTEM_SUPPORT_OS		
    static uint16_t fac_ms=0;				        //ms?車那㊣㊣?3?那y,?迆os??,∩迆㊣赤?????迆??米?ms那y
#endif
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  uint8_t c = (uint8_t)ch;
  HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
  return ch;
}


void Printf_Array(uint8_t *str, uint8_t *Array,uint16_t Len)
{
    printf("%s : ", str);

    for(int i = 0; i < Len; i++)
        printf("0x%02x, ", Array[i]);

    printf("\n");
}

static uint32_t g_delay_cycles_per_us = 0;

void delay_us_init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  g_delay_cycles_per_us = HAL_RCC_GetHCLKFreq() / 1000000U;
  if (g_delay_cycles_per_us == 0U)
  {
    g_delay_cycles_per_us = 1U;
  }
}

void delay_us(uint32_t us)
{
  if (us == 0U)
  {
    return;
  }
  if ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0U || g_delay_cycles_per_us == 0U)
  {
    delay_us_init();
  }

  uint64_t total_cycles_64 = (uint64_t)us * (uint64_t)g_delay_cycles_per_us;
  while (total_cycles_64 != 0U)
  {
    uint32_t chunk = (total_cycles_64 > 0xFFFFFFFFULL) ? 0xFFFFFFFFU : (uint32_t)total_cycles_64;
    uint32_t start = DWT->CYCCNT;
    while ((uint32_t)(DWT->CYCCNT - start) < chunk)
    {
    }
    total_cycles_64 -= (uint64_t)chunk;
  }
}

void delay_ms(uint32_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//?米赤3辰??-??DD
	{		
		//printf("%s: %d\r\n", __FILE__,__LINE__);
		if(nms>=fac_ms)						//?車那㊣米?那㊣??∩車車迆OS米?℅?谷迄那㊣???邦?迆 
		{ 
				//printf("%s: %d\r\n", __FILE__,__LINE__);
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS?車那㊣
		}
		nms%=fac_ms;						//OS辰??-?T﹞“足芍1??a?∩D?米??車那㊣芍?,2谷車???赤“﹞?那??車那㊣    
	}
	delay_us((uint32_t)(nms*1000));				//??赤“﹞?那??車那㊣
}

//?車那㊣nms,2??芍辰y?e豕???米¯?豕
//nms:辰a?車那㊣米?ms那y
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

uint16_t Get_GPIO_Pin_No(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
  case GPIO_PIN_0:
    return 0;
  case GPIO_PIN_1:
    return 1;
  case GPIO_PIN_2:
    return 2;
  case GPIO_PIN_3:
    return 3;
  case GPIO_PIN_4:
    return 4;
  case GPIO_PIN_5:
    return 5;
  case GPIO_PIN_6:
    return 6;
  case GPIO_PIN_7:
    return 7;
  case GPIO_PIN_8:
    return 8;
  case GPIO_PIN_9:
    return 9;
  case GPIO_PIN_10:
    return 10;
  case GPIO_PIN_11:
    return 11;
  case GPIO_PIN_12:
    return 12;
  case GPIO_PIN_13:
    return 13;
  case GPIO_PIN_14:
    return 14;
  case GPIO_PIN_15:
    return 15;
  default:
    return 0;
  }
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_UART4_Init();
//  MX_SPI6_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC3_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  delay_us_init();
  printf("app main\r\n");
  LAN_ZVPP_ECSUpper_Init();
  Init_ProtocolConvert();
  SysManage_CalcDeviceNum();

  USART_UART_Init(1, GetPortConfig_rs485(1));
  USART_UART_Init(2, GetPortConfig_rs485(2));
  USART_UART_Init(3, GetPortConfig_rs485(3));

  FDCAN_Init(1, GetPortConfig_CAN(1));
  FDCAN_Init(2, GetPortConfig_CAN(2));

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6|RCC_PERIPHCLK_SPI4
                              |RCC_PERIPHCLK_SPI5;
  PeriphClkInitStruct.PLL3.PLL3M = 2;
  PeriphClkInitStruct.PLL3.PLL3N = 120;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 5;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_PLL3;
  PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  static uint32_t wiz_timer_1ms_count = 0;
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM6) 
  {
    Task_Cycle_Count();
		
		wiz_timer_1ms_count++;
		if (wiz_timer_1ms_count >= 1000)
		{
				DHCP_time_handler();
				wiz_timer_1ms_count = 0;
		}
  }

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
