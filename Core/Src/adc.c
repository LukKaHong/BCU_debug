/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "cmsis_os.h"

/* USER CODE END 0 */

ADC_HandleTypeDef hadc3;

/* ADC3 init function */
void MX_ADC3_Init(void)
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
  HAL_ADCEx_Calibration_Start(&hadc3,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
  /* USER CODE END ADC3_Init 2 */

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.PLL2.PLL2M = 2;
    PeriphClkInitStruct.PLL2.PLL2N = 60;
    PeriphClkInitStruct.PLL2.PLL2P = 3;
    PeriphClkInitStruct.PLL2.PLL2Q = 2;
    PeriphClkInitStruct.PLL2.PLL2R = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
    PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* ADC3 clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF3     ------> ADC3_INP5
    PF4     ------> ADC3_INP9
    PH2     ------> ADC3_INP13
    PH3     ------> ADC3_INP14
    PH4     ------> ADC3_INP15
    PH5     ------> ADC3_INP16
    */
    GPIO_InitStruct.Pin = _2048mV_Pin|PWR_SAMP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RT1_SAMP_Pin|RT2_SAMP_Pin|Detect_Voltage1_Pin|Detect_Voltage2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();

    /**ADC3 GPIO Configuration
    PF3     ------> ADC3_INP5
    PF4     ------> ADC3_INP9
    PH2     ------> ADC3_INP13
    PH3     ------> ADC3_INP14
    PH4     ------> ADC3_INP15
    PH5     ------> ADC3_INP16
    */
    HAL_GPIO_DeInit(GPIOF, _2048mV_Pin|PWR_SAMP_Pin);

    HAL_GPIO_DeInit(GPIOH, RT1_SAMP_Pin|RT2_SAMP_Pin|Detect_Voltage1_Pin|Detect_Voltage2_Pin);

  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint16_t Get_Adc(ADC_HandleTypeDef *adc,uint32_t channel)
{
  uint16_t temp = 0;

  if(adc == &hadc3)
  {
    ADC_ChannelConfTypeDef ADC_ChanConf;

    ADC_ChanConf.Channel      = channel;
    ADC_ChanConf.Rank         = ADC_REGULAR_RANK_1;
    ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    ADC_ChanConf.SingleDiff   = ADC_SINGLE_ENDED;
    ADC_ChanConf.OffsetNumber = ADC_OFFSET_NONE;
    ADC_ChanConf.Offset       = 0;

    HAL_ADC_ConfigChannel(adc,&ADC_ChanConf);
    HAL_ADC_Start(adc); 
    HAL_ADC_PollForConversion(adc,10);
    temp = (uint16_t)HAL_ADC_GetValue(adc);
    HAL_ADC_Stop(adc);
  }

  return temp;
}

uint16_t ADC_GetAverage(ADC_HandleTypeDef *adc,uint32_t channel,uint8_t times)
{
  uint32_t temp_val = 0;
  uint8_t t;

  for(t = 0; t < times; t++)
  {
    temp_val += Get_Adc(adc, channel);
    osDelay(5);
  }
  return (uint16_t)(temp_val / times);
}

uint16_t ADC_GetAverage_Channel(ADCTYPE type)
{
  switch((uint8_t)type)
  {
    // case VREF_2048:
    //   return ADC_GetAverage(&hadc3, xx,3);
    // case PWR_SAMP:
    //   return ADC_GetAverage(&hadc3, xx,3);
    case RT1_SAMP:
      return ADC_GetAverage(&hadc3, ADC_CHANNEL_13, 3);
    // case RT2_SAMP:
    //   return ADC_GetAverage(&hadc3, xx,3);
    // case VOL1_SAMP:
    //   return ADC_GetAverage(&hadc3, xx,3);
    // case VOL2_SAMP:
    //   return ADC_GetAverage(&hadc3, xx,3);
    default:
      return 0;
  }
}
/* USER CODE END 1 */
