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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

SPI_HandleTypeDef hspi4;
SPI_HandleTypeDef hspi5;
SPI_HandleTypeDef hspi6;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;
DMA_HandleTypeDef hdma_spi6_tx;
DMA_HandleTypeDef hdma_spi6_rx;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Comm485_1_Task */
osThreadId_t Comm485_1_TaskHandle;
uint32_t Comm485_1_TaskBuffer[ 2048 ];
osStaticThreadDef_t Comm485_1_TaskControlBlock;
const osThreadAttr_t Comm485_1_Task_attributes = {
  .name = "Comm485_1_Task",
  .cb_mem = &Comm485_1_TaskControlBlock,
  .cb_size = sizeof(Comm485_1_TaskControlBlock),
  .stack_mem = &Comm485_1_TaskBuffer[0],
  .stack_size = sizeof(Comm485_1_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Comm485_2_Task */
osThreadId_t Comm485_2_TaskHandle;
uint32_t Comm485_2_TaskBuffer[ 2048 ];
osStaticThreadDef_t Comm485_2_TaskControlBlock;
const osThreadAttr_t Comm485_2_Task_attributes = {
  .name = "Comm485_2_Task",
  .cb_mem = &Comm485_2_TaskControlBlock,
  .cb_size = sizeof(Comm485_2_TaskControlBlock),
  .stack_mem = &Comm485_2_TaskBuffer[0],
  .stack_size = sizeof(Comm485_2_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for Comm485_3_Task */
osThreadId_t Comm485_3_TaskHandle;
uint32_t Comm485_3_TaskBuffer[ 2048 ];
osStaticThreadDef_t Comm485_3_TaskControlBlock;
const osThreadAttr_t Comm485_3_Task_attributes = {
  .name = "Comm485_3_Task",
  .cb_mem = &Comm485_3_TaskControlBlock,
  .cb_size = sizeof(Comm485_3_TaskControlBlock),
  .stack_mem = &Comm485_3_TaskBuffer[0],
  .stack_size = sizeof(Comm485_3_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal3,
};
/* Definitions for CommCAN_1_Task */
osThreadId_t CommCAN_1_TaskHandle;
uint32_t CommCAN_1_TaskBuffer[ 2048 ];
osStaticThreadDef_t CommCAN_1_TaskControlBlock;
const osThreadAttr_t CommCAN_1_Task_attributes = {
  .name = "CommCAN_1_Task",
  .cb_mem = &CommCAN_1_TaskControlBlock,
  .cb_size = sizeof(CommCAN_1_TaskControlBlock),
  .stack_mem = &CommCAN_1_TaskBuffer[0],
  .stack_size = sizeof(CommCAN_1_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal4,
};
/* Definitions for CommCAN_2_Task */
osThreadId_t CommCAN_2_TaskHandle;
uint32_t CommCAN_2_TaskBuffer[ 2048 ];
osStaticThreadDef_t CommCAN_2_TaskControlBlock;
const osThreadAttr_t CommCAN_2_Task_attributes = {
  .name = "CommCAN_2_Task",
  .cb_mem = &CommCAN_2_TaskControlBlock,
  .cb_size = sizeof(CommCAN_2_TaskControlBlock),
  .stack_mem = &CommCAN_2_TaskBuffer[0],
  .stack_size = sizeof(CommCAN_2_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal5,
};
/* Definitions for CommCAN_3_Task */
osThreadId_t CommCAN_3_TaskHandle;
uint32_t CommCAN_3_TaskBuffer[ 2048 ];
osStaticThreadDef_t CommCAN_3_TaskControlBlock;
const osThreadAttr_t CommCAN_3_Task_attributes = {
  .name = "CommCAN_3_Task",
  .cb_mem = &CommCAN_3_TaskControlBlock,
  .cb_size = sizeof(CommCAN_3_TaskControlBlock),
  .stack_mem = &CommCAN_3_TaskBuffer[0],
  .stack_size = sizeof(CommCAN_3_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal6,
};
/* Definitions for CommLAN_1_Task */
osThreadId_t CommLAN_1_TaskHandle;
uint32_t CommLAN_1_TaskBuffer[ 2048 ];
osStaticThreadDef_t CommLAN_1_TaskControlBlock;
const osThreadAttr_t CommLAN_1_Task_attributes = {
  .name = "CommLAN_1_Task",
  .cb_mem = &CommLAN_1_TaskControlBlock,
  .cb_size = sizeof(CommLAN_1_TaskControlBlock),
  .stack_mem = &CommLAN_1_TaskBuffer[0],
  .stack_size = sizeof(CommLAN_1_TaskBuffer),
  .priority = (osPriority_t) osPriorityNormal7,
};
/* Definitions for CommLAN_2_Task */
osThreadId_t CommLAN_2_TaskHandle;
uint32_t CommLAN_2_TaskBuffer[ 2048 ];
osStaticThreadDef_t CommLAN_2_TaskControlBlock;
const osThreadAttr_t CommLAN_2_Task_attributes = {
  .name = "CommLAN_2_Task",
  .cb_mem = &CommLAN_2_TaskControlBlock,
  .cb_size = sizeof(CommLAN_2_TaskControlBlock),
  .stack_mem = &CommLAN_2_TaskBuffer[0],
  .stack_size = sizeof(CommLAN_2_TaskBuffer),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for BinarySem_485_1_Tx */
osSemaphoreId_t BinarySem_485_1_TxHandle;
osStaticSemaphoreDef_t BinarySem_485_1_TxControlBlock;
const osSemaphoreAttr_t BinarySem_485_1_Tx_attributes = {
  .name = "BinarySem_485_1_Tx",
  .cb_mem = &BinarySem_485_1_TxControlBlock,
  .cb_size = sizeof(BinarySem_485_1_TxControlBlock),
};
/* Definitions for BinarySem_485_1_Rx */
osSemaphoreId_t BinarySem_485_1_RxHandle;
osStaticSemaphoreDef_t BinarySem_485_1_RxControlBlock;
const osSemaphoreAttr_t BinarySem_485_1_Rx_attributes = {
  .name = "BinarySem_485_1_Rx",
  .cb_mem = &BinarySem_485_1_RxControlBlock,
  .cb_size = sizeof(BinarySem_485_1_RxControlBlock),
};
/* Definitions for BinarySem_485_2_Tx */
osSemaphoreId_t BinarySem_485_2_TxHandle;
osStaticSemaphoreDef_t BinarySem_485_2_TxControlBlock;
const osSemaphoreAttr_t BinarySem_485_2_Tx_attributes = {
  .name = "BinarySem_485_2_Tx",
  .cb_mem = &BinarySem_485_2_TxControlBlock,
  .cb_size = sizeof(BinarySem_485_2_TxControlBlock),
};
/* Definitions for BinarySem_485_2_Rx */
osSemaphoreId_t BinarySem_485_2_RxHandle;
osStaticSemaphoreDef_t BinarySem_485_2_RxControlBlock;
const osSemaphoreAttr_t BinarySem_485_2_Rx_attributes = {
  .name = "BinarySem_485_2_Rx",
  .cb_mem = &BinarySem_485_2_RxControlBlock,
  .cb_size = sizeof(BinarySem_485_2_RxControlBlock),
};
/* Definitions for BinarySem_485_3_Tx */
osSemaphoreId_t BinarySem_485_3_TxHandle;
osStaticSemaphoreDef_t BinarySem_485_3_TxControlBlock;
const osSemaphoreAttr_t BinarySem_485_3_Tx_attributes = {
  .name = "BinarySem_485_3_Tx",
  .cb_mem = &BinarySem_485_3_TxControlBlock,
  .cb_size = sizeof(BinarySem_485_3_TxControlBlock),
};
/* Definitions for BinarySem_485_3_Rx */
osSemaphoreId_t BinarySem_485_3_RxHandle;
osStaticSemaphoreDef_t BinarySem_485_3_RxControlBlock;
const osSemaphoreAttr_t BinarySem_485_3_Rx_attributes = {
  .name = "BinarySem_485_3_Rx",
  .cb_mem = &BinarySem_485_3_RxControlBlock,
  .cb_size = sizeof(BinarySem_485_3_RxControlBlock),
};
/* Definitions for Comm485_1_Event */
osEventFlagsId_t Comm485_1_EventHandle;
osStaticEventGroupDef_t Comm485_1_EventControlBlock;
const osEventFlagsAttr_t Comm485_1_Event_attributes = {
  .name = "Comm485_1_Event",
  .cb_mem = &Comm485_1_EventControlBlock,
  .cb_size = sizeof(Comm485_1_EventControlBlock),
};
/* Definitions for Comm485_2_Event */
osEventFlagsId_t Comm485_2_EventHandle;
osStaticEventGroupDef_t Comm485_2_EventControlBlock;
const osEventFlagsAttr_t Comm485_2_Event_attributes = {
  .name = "Comm485_2_Event",
  .cb_mem = &Comm485_2_EventControlBlock,
  .cb_size = sizeof(Comm485_2_EventControlBlock),
};
/* Definitions for Comm485_3_Event */
osEventFlagsId_t Comm485_3_EventHandle;
osStaticEventGroupDef_t Comm485_3_EventControlBlock;
const osEventFlagsAttr_t Comm485_3_Event_attributes = {
  .name = "Comm485_3_Event",
  .cb_mem = &Comm485_3_EventControlBlock,
  .cb_size = sizeof(Comm485_3_EventControlBlock),
};
/* Definitions for CommCAN_1_Event */
osEventFlagsId_t CommCAN_1_EventHandle;
osStaticEventGroupDef_t CommCAN_1_EventControlBlock;
const osEventFlagsAttr_t CommCAN_1_Event_attributes = {
  .name = "CommCAN_1_Event",
  .cb_mem = &CommCAN_1_EventControlBlock,
  .cb_size = sizeof(CommCAN_1_EventControlBlock),
};
/* Definitions for CommCAN_2_Event */
osEventFlagsId_t CommCAN_2_EventHandle;
osStaticEventGroupDef_t CommCAN_2_EventControlBlock;
const osEventFlagsAttr_t CommCAN_2_Event_attributes = {
  .name = "CommCAN_2_Event",
  .cb_mem = &CommCAN_2_EventControlBlock,
  .cb_size = sizeof(CommCAN_2_EventControlBlock),
};
/* Definitions for CommCAN_3_Event */
osEventFlagsId_t CommCAN_3_EventHandle;
osStaticEventGroupDef_t CommCAN_3_EventControlBlock;
const osEventFlagsAttr_t CommCAN_3_Event_attributes = {
  .name = "CommCAN_3_Event",
  .cb_mem = &CommCAN_3_EventControlBlock,
  .cb_size = sizeof(CommCAN_3_EventControlBlock),
};
/* Definitions for CommLAN_1_Event */
osEventFlagsId_t CommLAN_1_EventHandle;
osStaticEventGroupDef_t CommLAN_1_EventControlBlock;
const osEventFlagsAttr_t CommLAN_1_Event_attributes = {
  .name = "CommLAN_1_Event",
  .cb_mem = &CommLAN_1_EventControlBlock,
  .cb_size = sizeof(CommLAN_1_EventControlBlock),
};
/* Definitions for CommLAN_2_Event */
osEventFlagsId_t CommLAN_2_EventHandle;
osStaticEventGroupDef_t CommLAN_2_EventControlBlock;
const osEventFlagsAttr_t CommLAN_2_Event_attributes = {
  .name = "CommLAN_2_Event",
  .cb_mem = &CommLAN_2_EventControlBlock,
  .cb_size = sizeof(CommLAN_2_EventControlBlock),
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_BDMA_Init(void);
static void MX_DMA_Init(void);
static void MX_UART4_Init(void);
static void MX_SPI6_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC3_Init(void);
static void MX_SPI4_Init(void);
static void MX_SPI5_Init(void);
void StartDefaultTask(void *argument);
void StartComm485_1_Task(void *argument);
void StartComm485_2_Task(void *argument);
void StartComm485_3_Task(void *argument);
void StartCommCAN_1_Task(void *argument);
void StartCommCAN_2_Task(void *argument);
void StartCommCAN_3_Task(void *argument);
void StartCommLAN_1_Task(void *argument);
void StartCommLAN_2_Task(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  uint8_t c = (uint8_t)ch;
  HAL_UART_Transmit(&huart2, &c, 1, HAL_MAX_DELAY);
  return ch;
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
/* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_BDMA_Init();
  MX_DMA_Init();
  MX_UART4_Init();
  MX_SPI6_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC3_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem_485_1_Tx */
  BinarySem_485_1_TxHandle = osSemaphoreNew(1, 0, &BinarySem_485_1_Tx_attributes);

  /* creation of BinarySem_485_1_Rx */
  BinarySem_485_1_RxHandle = osSemaphoreNew(1, 0, &BinarySem_485_1_Rx_attributes);

  /* creation of BinarySem_485_2_Tx */
  BinarySem_485_2_TxHandle = osSemaphoreNew(1, 0, &BinarySem_485_2_Tx_attributes);

  /* creation of BinarySem_485_2_Rx */
  BinarySem_485_2_RxHandle = osSemaphoreNew(1, 0, &BinarySem_485_2_Rx_attributes);

  /* creation of BinarySem_485_3_Tx */
  BinarySem_485_3_TxHandle = osSemaphoreNew(1, 0, &BinarySem_485_3_Tx_attributes);

  /* creation of BinarySem_485_3_Rx */
  BinarySem_485_3_RxHandle = osSemaphoreNew(1, 0, &BinarySem_485_3_Rx_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Comm485_1_Task */
  Comm485_1_TaskHandle = osThreadNew(StartComm485_1_Task, NULL, &Comm485_1_Task_attributes);

  /* creation of Comm485_2_Task */
  Comm485_2_TaskHandle = osThreadNew(StartComm485_2_Task, NULL, &Comm485_2_Task_attributes);

  /* creation of Comm485_3_Task */
  Comm485_3_TaskHandle = osThreadNew(StartComm485_3_Task, NULL, &Comm485_3_Task_attributes);

  /* creation of CommCAN_1_Task */
  CommCAN_1_TaskHandle = osThreadNew(StartCommCAN_1_Task, NULL, &CommCAN_1_Task_attributes);

  /* creation of CommCAN_2_Task */
  CommCAN_2_TaskHandle = osThreadNew(StartCommCAN_2_Task, NULL, &CommCAN_2_Task_attributes);

  /* creation of CommCAN_3_Task */
  CommCAN_3_TaskHandle = osThreadNew(StartCommCAN_3_Task, NULL, &CommCAN_3_Task_attributes);

  /* creation of CommLAN_1_Task */
  CommLAN_1_TaskHandle = osThreadNew(StartCommLAN_1_Task, NULL, &CommLAN_1_Task_attributes);

  /* creation of CommLAN_2_Task */
  CommLAN_2_TaskHandle = osThreadNew(StartCommLAN_2_Task, NULL, &CommLAN_2_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of Comm485_1_Event */
  Comm485_1_EventHandle = osEventFlagsNew(&Comm485_1_Event_attributes);

  /* creation of Comm485_2_Event */
  Comm485_2_EventHandle = osEventFlagsNew(&Comm485_2_Event_attributes);

  /* creation of Comm485_3_Event */
  Comm485_3_EventHandle = osEventFlagsNew(&Comm485_3_Event_attributes);

  /* creation of CommCAN_1_Event */
  CommCAN_1_EventHandle = osEventFlagsNew(&CommCAN_1_Event_attributes);

  /* creation of CommCAN_2_Event */
  CommCAN_2_EventHandle = osEventFlagsNew(&CommCAN_2_Event_attributes);

  /* creation of CommCAN_3_Event */
  CommCAN_3_EventHandle = osEventFlagsNew(&CommCAN_3_Event_attributes);

  /* creation of CommLAN_1_Event */
  CommLAN_1_EventHandle = osEventFlagsNew(&CommLAN_1_Event_attributes);

  /* creation of CommLAN_2_Event */
  CommLAN_2_EventHandle = osEventFlagsNew(&CommLAN_2_Event_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.Resolution = ADC_RESOLUTION_16B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 16;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 2;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 0;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 10;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 11;
  hfdcan2.Init.NominalTimeSeg2 = 8;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 1;
  hfdcan2.Init.DataTimeSeg2 = 1;
  hfdcan2.Init.MessageRAMOffset = 0;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.RxFifo0ElmtsNbr = 0;
  hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxFifo1ElmtsNbr = 0;
  hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxBuffersNbr = 0;
  hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.TxEventsNbr = 0;
  hfdcan2.Init.TxBuffersNbr = 0;
  hfdcan2.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 0x0;
  hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi4.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi4.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi4.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi4.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi4.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi4.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi4.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi4.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 0x0;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief SPI6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI6_Init(void)
{

  /* USER CODE BEGIN SPI6_Init 0 */

  /* USER CODE END SPI6_Init 0 */

  /* USER CODE BEGIN SPI6_Init 1 */

  /* USER CODE END SPI6_Init 1 */
  /* SPI6 parameter configuration*/
  hspi6.Instance = SPI6;
  hspi6.Init.Mode = SPI_MODE_MASTER;
  hspi6.Init.Direction = SPI_DIRECTION_2LINES;
  hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi6.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi6.Init.NSS = SPI_NSS_SOFT;
  hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi6.Init.CRCPolynomial = 0x0;
  hspi6.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi6.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI6_Init 2 */

  /* USER CODE END SPI6_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_BDMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* DMA interrupt init */
  /* BDMA_Channel0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BDMA_Channel0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(BDMA_Channel0_IRQn);
  /* BDMA_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BDMA_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(BDMA_Channel1_IRQn);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, Relay1_Pin|Relay2_Pin|Relay3_Pin|Relay4_Pin
                          |Relay5_Pin|W5500_RSTn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, RTC_I2C1_SDA_Pin|RTC_I2C1_SCL_Pin|Flash_SPI_CS_Pin|EEPROM_WP_Pin
                          |EEPROM_SCL_Pin|EEPROM_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, WDI_Pin|LED_DEBUG_Pin|A_W5500_RSTn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(A_W5500_SCSn_GPIO_Port, A_W5500_SCSn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(W5500_SCSn_GPIO_Port, W5500_SCSn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : Relay1_Pin Relay2_Pin Relay3_Pin Relay4_Pin
                           Relay5_Pin W5500_SCSn_Pin W5500_RSTn_Pin */
  GPIO_InitStruct.Pin = Relay1_Pin|Relay2_Pin|Relay3_Pin|Relay4_Pin
                          |Relay5_Pin|W5500_SCSn_Pin|W5500_RSTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : Relay6_Pin */
  GPIO_InitStruct.Pin = Relay6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Relay6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RTC_I2C1_SDA_Pin RTC_I2C1_SCL_Pin Flash_SPI_CS_Pin EEPROM_WP_Pin
                           EEPROM_SCL_Pin EEPROM_SDA_Pin */
  GPIO_InitStruct.Pin = RTC_I2C1_SDA_Pin|RTC_I2C1_SCL_Pin|Flash_SPI_CS_Pin|EEPROM_WP_Pin
                          |EEPROM_SCL_Pin|EEPROM_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : RTC_EN_Pin */
  GPIO_InitStruct.Pin = RTC_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RTC_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : WDI_Pin LED_DEBUG_Pin */
  GPIO_InitStruct.Pin = WDI_Pin|LED_DEBUG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : A_W5500_SCSn_Pin */
  GPIO_InitStruct.Pin = A_W5500_SCSn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(A_W5500_SCSn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : A_W5500_INTn_Pin */
  GPIO_InitStruct.Pin = A_W5500_INTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(A_W5500_INTn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : A_W5500_RSTn_Pin */
  GPIO_InitStruct.Pin = A_W5500_RSTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(A_W5500_RSTn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : W5500_INTn_Pin */
  GPIO_InitStruct.Pin = W5500_INTn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(W5500_INTn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Switch_Detection8_Pin Switch_Detection7_Pin Switch_Detection6_Pin */
  GPIO_InitStruct.Pin = Switch_Detection8_Pin|Switch_Detection7_Pin|Switch_Detection6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /*Configure GPIO pins : Switch_Detection5_Pin Switch_Detection4_Pin Switch_Detection3_Pin Switch_Detection2_Pin
                           Switch_Detection1_Pin */
  GPIO_InitStruct.Pin = Switch_Detection5_Pin|Switch_Detection4_Pin|Switch_Detection3_Pin|Switch_Detection2_Pin
                          |Switch_Detection1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : VNC_A_Pin VNC_B_Pin VNC_C_Pin */
  GPIO_InitStruct.Pin = VNC_A_Pin|VNC_B_Pin|VNC_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
    printf("__FUNC__: %s\r\n", __func__);
    HAL_UART_Transmit(&huart1, "huart1", 6, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, "huart3", 6, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart4, "huart4", 6, HAL_MAX_DELAY);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartComm485_1_Task */
/**
* @brief Function implementing the Comm485_1_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartComm485_1_Task */
void StartComm485_1_Task(void *argument)
{
  /* USER CODE BEGIN StartComm485_1_Task */
  /* Infinite loop */
  Comm485_1_Task();
  /* USER CODE END StartComm485_1_Task */
}

/* USER CODE BEGIN Header_StartComm485_2_Task */
/**
* @brief Function implementing the Comm485_2_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartComm485_2_Task */
void StartComm485_2_Task(void *argument)
{
  /* USER CODE BEGIN StartComm485_2_Task */
  /* Infinite loop */
  Comm485_2_Task();
  /* USER CODE END StartComm485_2_Task */
}

/* USER CODE BEGIN Header_StartComm485_3_Task */
/**
* @brief Function implementing the Comm485_3_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartComm485_3_Task */
void StartComm485_3_Task(void *argument)
{
  /* USER CODE BEGIN StartComm485_3_Task */
  /* Infinite loop */
  Comm485_3_Task();
  /* USER CODE END StartComm485_3_Task */
}

/* USER CODE BEGIN Header_StartCommCAN_1_Task */
/**
* @brief Function implementing the CommCAN_1_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommCAN_1_Task */
void StartCommCAN_1_Task(void *argument)
{
  /* USER CODE BEGIN StartCommCAN_1_Task */
  /* Infinite loop */
  CommCAN_1_Task();
  /* USER CODE END StartCommCAN_1_Task */
}

/* USER CODE BEGIN Header_StartCommCAN_2_Task */
/**
* @brief Function implementing the CommCAN_2_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommCAN_2_Task */
void StartCommCAN_2_Task(void *argument)
{
  /* USER CODE BEGIN StartCommCAN_2_Task */
  /* Infinite loop */
  CommCAN_2_Task();
  /* USER CODE END StartCommCAN_2_Task */
}

/* USER CODE BEGIN Header_StartCommCAN_3_Task */
/**
* @brief Function implementing the CommCAN_3_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommCAN_3_Task */
void StartCommCAN_3_Task(void *argument)
{
  /* USER CODE BEGIN StartCommCAN_3_Task */
  /* Infinite loop */
  CommCAN_3_Task();
  /* USER CODE END StartCommCAN_3_Task */
}

/* USER CODE BEGIN Header_StartCommLAN_1_Task */
/**
* @brief Function implementing the CommLAN_1_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommLAN_1_Task */
void StartCommLAN_1_Task(void *argument)
{
  /* USER CODE BEGIN StartCommLAN_1_Task */
  /* Infinite loop */
  CommLAN_1_Task();
  /* USER CODE END StartCommLAN_1_Task */
}

/* USER CODE BEGIN Header_StartCommLAN_2_Task */
/**
* @brief Function implementing the CommLAN_2_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommLAN_2_Task */
void StartCommLAN_2_Task(void *argument)
{
  /* USER CODE BEGIN StartCommLAN_2_Task */
  /* Infinite loop */
  CommLAN_2_Task();
  /* USER CODE END StartCommLAN_2_Task */
}

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

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

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
