/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define DEN3_Pin GPIO_PIN_13
#define DEN3_GPIO_Port GPIOC
#define DEN2_Pin GPIO_PIN_14
#define DEN2_GPIO_Port GPIOC
#define DEN1_Pin GPIO_PIN_15
#define DEN1_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define QUAT1_Pin GPIO_PIN_0
#define QUAT1_GPIO_Port GPIOB
#define QUAT2_Pin GPIO_PIN_1
#define QUAT2_GPIO_Port GPIOB
#define QUAT3_Pin GPIO_PIN_10
#define QUAT3_GPIO_Port GPIOB
#define COI_Pin GPIO_PIN_11
#define COI_GPIO_Port GPIOB
#define NUT1_Pin GPIO_PIN_12
#define NUT1_GPIO_Port GPIOB
#define NUT2_Pin GPIO_PIN_13
#define NUT2_GPIO_Port GPIOB
#define NUT3_Pin GPIO_PIN_14
#define NUT3_GPIO_Port GPIOB
#define NUT4_Pin GPIO_PIN_15
#define NUT4_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_8
#define DHT11_GPIO_Port GPIOA
#define NUT5_Pin GPIO_PIN_11
#define NUT5_GPIO_Port GPIOA
#define NUT6_Pin GPIO_PIN_12
#define NUT6_GPIO_Port GPIOA
#define NUT7_Pin GPIO_PIN_15
#define NUT7_GPIO_Port GPIOA
#define HNDOOR_Pin GPIO_PIN_3
#define HNDOOR_GPIO_Port GPIOB
#define BUTTON_Pin GPIO_PIN_4
#define BUTTON_GPIO_Port GPIOB
#define HNTOLET_Pin GPIO_PIN_5
#define HNTOLET_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOB
#define DEN4_Pin GPIO_PIN_9
#define DEN4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
