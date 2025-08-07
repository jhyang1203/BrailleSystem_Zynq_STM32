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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define CSN_Pin GPIO_PIN_1
#define CSN_GPIO_Port GPIOC
#define CE_Pin GPIO_PIN_4
#define CE_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define sw3_Pin GPIO_PIN_5
#define sw3_GPIO_Port GPIOC
#define btn_start_Pin GPIO_PIN_2
#define btn_start_GPIO_Port GPIOB
#define busy_Pin GPIO_PIN_10
#define busy_GPIO_Port GPIOB
#define sw6_Pin GPIO_PIN_12
#define sw6_GPIO_Port GPIOB
#define sw1_Pin GPIO_PIN_8
#define sw1_GPIO_Port GPIOC
#define sw2_Pin GPIO_PIN_9
#define sw2_GPIO_Port GPIOC
#define IRQ_Pin GPIO_PIN_10
#define IRQ_GPIO_Port GPIOA
#define IRQ_EXTI_IRQn EXTI15_10_IRQn
#define sw5_Pin GPIO_PIN_11
#define sw5_GPIO_Port GPIOA
#define sw4_Pin GPIO_PIN_12
#define sw4_GPIO_Port GPIOA
#define btn_volumeup_Pin GPIO_PIN_8
#define btn_volumeup_GPIO_Port GPIOB
#define btn_volumedown_Pin GPIO_PIN_9
#define btn_volumedown_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
