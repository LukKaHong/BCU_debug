/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
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
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan1;

extern FDCAN_HandleTypeDef hfdcan2;

/* USER CODE BEGIN Private defines */
#define CAN_SendBuff_Max (100)
#define CAN_ReceiveBuff_Max (100)

typedef struct
{
    uint32_t id;        /* CAN Msg id, donnot include SRR IDE RTR */
    uint8_t length;     /* CAN Msg data length */
    uint8_t data[8];    /* CAN Msg data */
}CanMsgType;

typedef struct
{
  CanMsgType Msg[CAN_SendBuff_Max];
  uint8_t TxIndex;
  uint8_t CurIndex;
}tCAN_SendBuff;

typedef struct
{
  CanMsgType Msg[CAN_ReceiveBuff_Max];
  uint8_t RxIndex;
  uint8_t CurIndex;
}tCAN_ReceiveBuff;




/* USER CODE END Private defines */

void MX_FDCAN1_Init(void);
void MX_FDCAN2_Init(void);

/* USER CODE BEGIN Prototypes */
extern HAL_StatusTypeDef FDCAN1_Send_Msg(CanMsgType *msg);
extern HAL_StatusTypeDef FDCAN2_Send_Msg(CanMsgType *msg);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

