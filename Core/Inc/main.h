/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define MINIMP3_MAX_SAMPLES_PER_FRAME (1152*2)
#define DMA_BUFFER_SIZE (1152*3)

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern SD_HandleTypeDef hsd;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim6;

extern signed short samples[];
extern uint16_t samples_DMA[];

extern uint8_t audioPlaying;
extern uint8_t spiTransmitComplete;
extern uint32_t dmaLoopsCompleted;
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
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_6
#define TFT_DC_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOF
#define TFT_RST_Pin GPIO_PIN_8
#define TFT_RST_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
