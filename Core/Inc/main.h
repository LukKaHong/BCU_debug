/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Relay1_Pin GPIO_PIN_2
#define Relay1_GPIO_Port GPIOE
#define Relay2_Pin GPIO_PIN_3
#define Relay2_GPIO_Port GPIOE
#define Relay3_Pin GPIO_PIN_4
#define Relay3_GPIO_Port GPIOE
#define Relay4_Pin GPIO_PIN_5
#define Relay4_GPIO_Port GPIOE
#define Relay5_Pin GPIO_PIN_6
#define Relay5_GPIO_Port GPIOE
#define Relay6_Pin GPIO_PIN_8
#define Relay6_GPIO_Port GPIOI
#define RTC_I2C1_SDA_Pin GPIO_PIN_0
#define RTC_I2C1_SDA_GPIO_Port GPIOF
#define RTC_I2C1_SCL_Pin GPIO_PIN_1
#define RTC_I2C1_SCL_GPIO_Port GPIOF
#define RTC_EN_Pin GPIO_PIN_2
#define RTC_EN_GPIO_Port GPIOF
#define _2048mV_Pin GPIO_PIN_3
#define _2048mV_GPIO_Port GPIOF
#define PWR_SAMP_Pin GPIO_PIN_4
#define PWR_SAMP_GPIO_Port GPIOF
#define Flash_SPI_CS_Pin GPIO_PIN_6
#define Flash_SPI_CS_GPIO_Port GPIOF
#define Flash_SPI_CLK_Pin GPIO_PIN_7
#define Flash_SPI_CLK_GPIO_Port GPIOF
#define Flash_SPI_MISO_Pin GPIO_PIN_8
#define Flash_SPI_MISO_GPIO_Port GPIOF
#define Flash_SPI_MOSI_Pin GPIO_PIN_9
#define Flash_SPI_MOSI_GPIO_Port GPIOF
#define WDI_Pin GPIO_PIN_0
#define WDI_GPIO_Port GPIOC
#define LED_DEBUG_Pin GPIO_PIN_1
#define LED_DEBUG_GPIO_Port GPIOC
#define RT1_SAMP_Pin GPIO_PIN_2
#define RT1_SAMP_GPIO_Port GPIOH
#define RT2_SAMP_Pin GPIO_PIN_3
#define RT2_SAMP_GPIO_Port GPIOH
#define Detect_Voltage1_Pin GPIO_PIN_4
#define Detect_Voltage1_GPIO_Port GPIOH
#define Detect_Voltage2_Pin GPIO_PIN_5
#define Detect_Voltage2_GPIO_Port GPIOH
#define A_W5500_SCSn_Pin GPIO_PIN_4
#define A_W5500_SCSn_GPIO_Port GPIOA
#define A_W5500_SCLK_Pin GPIO_PIN_5
#define A_W5500_SCLK_GPIO_Port GPIOA
#define A_W5500_MISO_Pin GPIO_PIN_6
#define A_W5500_MISO_GPIO_Port GPIOA
#define A_W5500_MOSI_Pin GPIO_PIN_7
#define A_W5500_MOSI_GPIO_Port GPIOA
#define A_W5500_INTn_Pin GPIO_PIN_4
#define A_W5500_INTn_GPIO_Port GPIOC
#define A_W5500_RSTn_Pin GPIO_PIN_5
#define A_W5500_RSTn_GPIO_Port GPIOC
#define EEPROM_WP_Pin GPIO_PIN_13
#define EEPROM_WP_GPIO_Port GPIOF
#define EEPROM_SCL_Pin GPIO_PIN_14
#define EEPROM_SCL_GPIO_Port GPIOF
#define EEPROM_SDA_Pin GPIO_PIN_15
#define EEPROM_SDA_GPIO_Port GPIOF
#define W5500_INTn_Pin GPIO_PIN_10
#define W5500_INTn_GPIO_Port GPIOE
#define W5500_SCSn_Pin GPIO_PIN_11
#define W5500_SCSn_GPIO_Port GPIOE
#define W5500_SCLK_Pin GPIO_PIN_12
#define W5500_SCLK_GPIO_Port GPIOE
#define W5500_MISO_Pin GPIO_PIN_13
#define W5500_MISO_GPIO_Port GPIOE
#define W5500_MOSI_Pin GPIO_PIN_14
#define W5500_MOSI_GPIO_Port GPIOE
#define W5500_RSTn_Pin GPIO_PIN_15
#define W5500_RSTn_GPIO_Port GPIOE
#define Switch_Detection8_Pin GPIO_PIN_0
#define Switch_Detection8_GPIO_Port GPIOK
#define Switch_Detection7_Pin GPIO_PIN_1
#define Switch_Detection7_GPIO_Port GPIOK
#define Switch_Detection6_Pin GPIO_PIN_2
#define Switch_Detection6_GPIO_Port GPIOK
#define Switch_Detection5_Pin GPIO_PIN_2
#define Switch_Detection5_GPIO_Port GPIOG
#define Switch_Detection4_Pin GPIO_PIN_3
#define Switch_Detection4_GPIO_Port GPIOG
#define Switch_Detection3_Pin GPIO_PIN_4
#define Switch_Detection3_GPIO_Port GPIOG
#define Switch_Detection2_Pin GPIO_PIN_5
#define Switch_Detection2_GPIO_Port GPIOG
#define Switch_Detection1_Pin GPIO_PIN_6
#define Switch_Detection1_GPIO_Port GPIOG
#define Tx_485_2_Pin GPIO_PIN_9
#define Tx_485_2_GPIO_Port GPIOA
#define Rx_485_2_Pin GPIO_PIN_10
#define Rx_485_2_GPIO_Port GPIOA
#define _485_DE2_Pin GPIO_PIN_11
#define _485_DE2_GPIO_Port GPIOA
#define _485_DE3_Pin GPIO_PIN_3
#define _485_DE3_GPIO_Port GPIOI
#define Tx_485_3_Pin GPIO_PIN_10
#define Tx_485_3_GPIO_Port GPIOC
#define Rx_485_3_Pin GPIO_PIN_11
#define Rx_485_3_GPIO_Port GPIOC
#define _485_DE1_Pin GPIO_PIN_12
#define _485_DE1_GPIO_Port GPIOC
#define Rx_485_1_Pin GPIO_PIN_0
#define Rx_485_1_GPIO_Port GPIOD
#define Tx_485_1_Pin GPIO_PIN_1
#define Tx_485_1_GPIO_Port GPIOD
#define Tx_Debug_Pin GPIO_PIN_5
#define Tx_Debug_GPIO_Port GPIOD
#define Rx_Debug_Pin GPIO_PIN_6
#define Rx_Debug_GPIO_Port GPIOD
#define VNC_A_Pin GPIO_PIN_5
#define VNC_A_GPIO_Port GPIOI
#define VNC_B_Pin GPIO_PIN_6
#define VNC_B_GPIO_Port GPIOI
#define VNC_C_Pin GPIO_PIN_7
#define VNC_C_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */
extern void Printf_Array(uint8_t *str, uint8_t *Array,uint16_t Len);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
