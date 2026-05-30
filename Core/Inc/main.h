/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Motor_Enable_Pin GPIO_PIN_13
#define Motor_Enable_GPIO_Port GPIOC
#define Motor_Sleep_Pin GPIO_PIN_14
#define Motor_Sleep_GPIO_Port GPIOC
#define Motor_4_Reset_Pin GPIO_PIN_0
#define Motor_4_Reset_GPIO_Port GPIOC
#define Motor_6_Reset_Pin GPIO_PIN_1
#define Motor_6_Reset_GPIO_Port GPIOC
#define Motor_5_Reset_Pin GPIO_PIN_2
#define Motor_5_Reset_GPIO_Port GPIOC
#define VERT_Pin GPIO_PIN_0
#define VERT_GPIO_Port GPIOA
#define HORIX_Pin GPIO_PIN_1
#define HORIX_GPIO_Port GPIOA
#define SEL_Pin GPIO_PIN_2
#define SEL_GPIO_Port GPIOA
#define MP2_NSCS_Pin GPIO_PIN_4
#define MP2_NSCS_GPIO_Port GPIOC
#define MP1_NINT_Pin GPIO_PIN_5
#define MP1_NINT_GPIO_Port GPIOC
#define MP1_NINT_EXTI_IRQn EXTI9_5_IRQn
#define MP1_NSCS_Pin GPIO_PIN_0
#define MP1_NSCS_GPIO_Port GPIOB
#define MP2_NINT_Pin GPIO_PIN_1
#define MP2_NINT_GPIO_Port GPIOB
#define Motor_Decay1_Pin GPIO_PIN_12
#define Motor_Decay1_GPIO_Port GPIOB
#define Motor_Decay_2_Pin GPIO_PIN_13
#define Motor_Decay_2_GPIO_Port GPIOB
#define Motor_1_Reset_Pin GPIO_PIN_6
#define Motor_1_Reset_GPIO_Port GPIOC
#define Motor_2_Reset_Pin GPIO_PIN_7
#define Motor_2_Reset_GPIO_Port GPIOC
#define Motor_2_Reset_EXTI_IRQn EXTI9_5_IRQn
#define Motor_3_Reset_Pin GPIO_PIN_9
#define Motor_3_Reset_GPIO_Port GPIOC
#define MP_CLK_Pin GPIO_PIN_8
#define MP_CLK_GPIO_Port GPIOA
#define USB_D__Pin GPIO_PIN_11
#define USB_D__GPIO_Port GPIOA
#define USB_D_A12_Pin GPIO_PIN_12
#define USB_D_A12_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
