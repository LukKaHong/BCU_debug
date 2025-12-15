/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "CommCAN.h"
#include "CommLAN_1.h"
#include "CommLAN_2.h"
#include "Comm485.h"
#include "DI.h"
#include "AD.h"



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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
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
/* Definitions for DI_Task */
osThreadId_t DI_TaskHandle;
uint32_t DI_TaskBuffer[ 2048 ];
osStaticThreadDef_t DI_TaskControlBlock;
const osThreadAttr_t DI_Task_attributes = {
  .name = "DI_Task",
  .cb_mem = &DI_TaskControlBlock,
  .cb_size = sizeof(DI_TaskControlBlock),
  .stack_mem = &DI_TaskBuffer[0],
  .stack_size = sizeof(DI_TaskBuffer),
  .priority = (osPriority_t) osPriorityBelowNormal7,
};
/* Definitions for ADC_Task */
osThreadId_t ADC_TaskHandle;
uint32_t ADC_TaskBuffer[ 2048 ];
osStaticThreadDef_t ADC_TaskControlBlock;
const osThreadAttr_t ADC_Task_attributes = {
  .name = "ADC_Task",
  .cb_mem = &ADC_TaskControlBlock,
  .cb_size = sizeof(ADC_TaskControlBlock),
  .stack_mem = &ADC_TaskBuffer[0],
  .stack_size = sizeof(ADC_TaskBuffer),
  .priority = (osPriority_t) osPriorityBelowNormal6,
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
/* Definitions for BinarySem_LAN_2_Tx */
osSemaphoreId_t BinarySem_LAN_2_TxHandle;
osStaticSemaphoreDef_t BinarySem_LAN_2_TxControlBlock;
const osSemaphoreAttr_t BinarySem_LAN_2_Tx_attributes = {
  .name = "BinarySem_LAN_2_Tx",
  .cb_mem = &BinarySem_LAN_2_TxControlBlock,
  .cb_size = sizeof(BinarySem_LAN_2_TxControlBlock),
};
/* Definitions for BinarySem_LAN_2_Rx */
osSemaphoreId_t BinarySem_LAN_2_RxHandle;
osStaticSemaphoreDef_t BinarySem_LAN_2_RxControlBlock;
const osSemaphoreAttr_t BinarySem_LAN_2_Rx_attributes = {
  .name = "BinarySem_LAN_2_Rx",
  .cb_mem = &BinarySem_LAN_2_RxControlBlock,
  .cb_size = sizeof(BinarySem_LAN_2_RxControlBlock),
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
/* Definitions for DI_Event */
osEventFlagsId_t DI_EventHandle;
osStaticEventGroupDef_t DI_EventControlBlock;
const osEventFlagsAttr_t DI_Event_attributes = {
  .name = "DI_Event",
  .cb_mem = &DI_EventControlBlock,
  .cb_size = sizeof(DI_EventControlBlock),
};
/* Definitions for ADC_Event */
osEventFlagsId_t ADC_EventHandle;
osStaticEventGroupDef_t ADC_EventControlBlock;
const osEventFlagsAttr_t ADC_Event_attributes = {
  .name = "ADC_Event",
  .cb_mem = &ADC_EventControlBlock,
  .cb_size = sizeof(ADC_EventControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartComm485_1_Task(void *argument);
void StartComm485_2_Task(void *argument);
void StartComm485_3_Task(void *argument);
void StartCommCAN_1_Task(void *argument);
void StartCommCAN_2_Task(void *argument);
void StartCommCAN_3_Task(void *argument);
void StartCommLAN_1_Task(void *argument);
void StartCommLAN_2_Task(void *argument);
void StartDI_Task(void *argument);
void StartADC_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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

  /* creation of BinarySem_LAN_2_Tx */
  BinarySem_LAN_2_TxHandle = osSemaphoreNew(1, 0, &BinarySem_LAN_2_Tx_attributes);

  /* creation of BinarySem_LAN_2_Rx */
  BinarySem_LAN_2_RxHandle = osSemaphoreNew(1, 0, &BinarySem_LAN_2_Rx_attributes);

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

  /* creation of DI_Task */
  DI_TaskHandle = osThreadNew(StartDI_Task, NULL, &DI_Task_attributes);

  /* creation of ADC_Task */
  ADC_TaskHandle = osThreadNew(StartADC_Task, NULL, &ADC_Task_attributes);

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

  /* creation of DI_Event */
  DI_EventHandle = osEventFlagsNew(&DI_Event_attributes);

  /* creation of ADC_Event */
  ADC_EventHandle = osEventFlagsNew(&ADC_Event_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
    // printf("__FUNC__: %s\n", __func__);
    // HAL_UART_Transmit(&huart1, (uint8_t *)"huart1\n", 8, HAL_MAX_DELAY);
    // HAL_UART_Transmit(&huart3, (uint8_t *)"huart3\n", 8, HAL_MAX_DELAY);
    // HAL_UART_Transmit(&huart4, (uint8_t *)"huart4\n", 8, HAL_MAX_DELAY);
  }
  /* USER CODE END StartDefaultTask */
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

/* USER CODE BEGIN Header_StartDI_Task */
/**
* @brief Function implementing the DI_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDI_Task */
void StartDI_Task(void *argument)
{
  /* USER CODE BEGIN StartDI_Task */
  /* Infinite loop */
  DI_Task();
  /* USER CODE END StartDI_Task */
}

/* USER CODE BEGIN Header_StartADC_Task */
/**
* @brief Function implementing the ADC_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartADC_Task */
void StartADC_Task(void *argument)
{
  /* USER CODE BEGIN StartADC_Task */
  /* Infinite loop */
  ADC_Task();
  /* USER CODE END StartADC_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Task_Cycle_Count(void)
{
  static uint16_t Comm485_1_tick = 0;
  static uint16_t Comm485_2_tick = 0;
  static uint16_t Comm485_3_tick = 0;
  static uint16_t CommCAN_1_tick = 0;
  static uint16_t CommCAN_2_tick = 0;
  static uint16_t CommCAN_3_tick = 0;
  static uint16_t CommLAN_1_tick = 0;
  static uint16_t CommLAN_2_tick = 0;
  static uint16_t DI_tick = 0;
  static uint16_t ADC_tick = 0;

  if(++Comm485_1_tick >= Comm485_Task_Cycle)
  {
    Comm485_1_tick = 0;
    osEventFlagsSet(Comm485_1_EventHandle, Comm485_1_Event_Tick);
  }

  if(++Comm485_2_tick >= Comm485_Task_Cycle)
  {
    Comm485_2_tick = 0;
    osEventFlagsSet(Comm485_2_EventHandle, Comm485_2_Event_Tick);
  }

  if(++Comm485_3_tick >= Comm485_Task_Cycle)
  {
    Comm485_3_tick = 0;
    osEventFlagsSet(Comm485_3_EventHandle, Comm485_3_Event_Tick);
  }

  if(++CommCAN_1_tick >= CommCAN_Task_Cycle)
  {
    CommCAN_1_tick = 0;
    osEventFlagsSet(CommCAN_1_EventHandle, CommCAN_1_Event_Tick);
  }

  if(++CommCAN_2_tick >= CommCAN_Task_Cycle)
  {
    CommCAN_2_tick = 0;
    osEventFlagsSet(CommCAN_2_EventHandle, CommCAN_2_Event_Tick);
  }

  if(++CommCAN_3_tick >= CommCAN_Task_Cycle)
  {
    CommCAN_3_tick = 0;
    osEventFlagsSet(CommCAN_3_EventHandle, CommCAN_3_Event_Tick);
  }

  if(++CommLAN_1_tick >= CommLAN_1_Task_Cycle)
  {
    CommLAN_1_tick = 0;
    osEventFlagsSet(CommLAN_1_EventHandle, CommLAN_1_Event_Tick);
  }

  if(++CommLAN_2_tick >= CommLAN_2_Task_Cycle)
  {
    CommLAN_2_tick = 0;
    osEventFlagsSet(CommLAN_2_EventHandle, CommLAN_2_Event_Tick);
  }

  if(++DI_tick >= DI_Task_Cycle)
  {
    DI_tick = 0;
    osEventFlagsSet(DI_EventHandle, DI_Event_Tick);
  }

  if(++ADC_tick >= ADC_Task_Cycle)
  {
    ADC_tick = 0;
    osEventFlagsSet(ADC_EventHandle, ADC_Event_Tick);
  }

}


/* USER CODE END Application */

