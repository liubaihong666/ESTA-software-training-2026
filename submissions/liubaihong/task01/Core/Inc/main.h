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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CH1_ADG409_A0_Pin GPIO_PIN_2
#define CH1_ADG409_A0_GPIO_Port GPIOE
#define CH2_IN_ADG408_A1_Pin GPIO_PIN_4
#define CH2_IN_ADG408_A1_GPIO_Port GPIOE
#define CH1_ADG408_A2_Pin GPIO_PIN_5
#define CH1_ADG408_A2_GPIO_Port GPIOE
#define ILI9341_SDI_Pin GPIO_PIN_0
#define ILI9341_SDI_GPIO_Port GPIOC
#define ILI9341_RS_Pin GPIO_PIN_2
#define ILI9341_RS_GPIO_Port GPIOC
#define CH2_Pin GPIO_PIN_0
#define CH2_GPIO_Port GPIOA
#define CH2_IN_ADG408_A0_Pin GPIO_PIN_1
#define CH2_IN_ADG408_A0_GPIO_Port GPIOA
#define CH2_ADG408_A2_Pin GPIO_PIN_3
#define CH2_ADG408_A2_GPIO_Port GPIOA
#define CH1_Pin GPIO_PIN_6
#define CH1_GPIO_Port GPIOA
#define KEY_ENCODER_Pin GPIO_PIN_4
#define KEY_ENCODER_GPIO_Port GPIOC
#define KEY_ENCODER_EXTI_IRQn EXTI4_IRQn
#define KEY_AUTO_Pin GPIO_PIN_5
#define KEY_AUTO_GPIO_Port GPIOC
#define KEY_AUTO_EXTI_IRQn EXTI9_5_IRQn
#define RISING_EDGE_TRIGGERING_Pin GPIO_PIN_0
#define RISING_EDGE_TRIGGERING_GPIO_Port GPIOB
#define RISING_EDGE_TRIGGERING_EXTI_IRQn EXTI0_IRQn
#define KEY_SINGLE_Pin GPIO_PIN_1
#define KEY_SINGLE_GPIO_Port GPIOB
#define KEY_SINGLE_EXTI_IRQn EXTI1_IRQn
#define CH2_ADG409_A0_Pin GPIO_PIN_2
#define CH2_ADG409_A0_GPIO_Port GPIOB
#define KEY_MODE_Pin GPIO_PIN_7
#define KEY_MODE_GPIO_Port GPIOE
#define KEY_MODE_EXTI_IRQn EXTI9_5_IRQn
#define KEY_CH_TOGGLE_Pin GPIO_PIN_8
#define KEY_CH_TOGGLE_GPIO_Port GPIOE
#define KEY_CH_TOGGLE_EXTI_IRQn EXTI9_5_IRQn
#define CH1_IN_ADG408_A0_Pin GPIO_PIN_11
#define CH1_IN_ADG408_A0_GPIO_Port GPIOD
#define CH1_ADG409_A1_Pin GPIO_PIN_12
#define CH1_ADG409_A1_GPIO_Port GPIOD
#define CH2_ADG409_A1_Pin GPIO_PIN_13
#define CH2_ADG409_A1_GPIO_Port GPIOD
#define ILI9341_CS_Pin GPIO_PIN_7
#define ILI9341_CS_GPIO_Port GPIOC
#define CH2_ADG408_A0_Pin GPIO_PIN_8
#define CH2_ADG408_A0_GPIO_Port GPIOC
#define CH2_ADG408_A1_Pin GPIO_PIN_9
#define CH2_ADG408_A1_GPIO_Port GPIOC
#define ILI9341_RST_Pin GPIO_PIN_10
#define ILI9341_RST_GPIO_Port GPIOA
#define CH1_IN_ADG408_A1_Pin GPIO_PIN_10
#define CH1_IN_ADG408_A1_GPIO_Port GPIOC
#define CH1_ADG408_A1_Pin GPIO_PIN_12
#define CH1_ADG408_A1_GPIO_Port GPIOC
#define ILI9341_SCK_Pin GPIO_PIN_0
#define ILI9341_SCK_GPIO_Port GPIOD
#define CH1_ADG408_A0_Pin GPIO_PIN_2
#define CH1_ADG408_A0_GPIO_Port GPIOD
#define ILI9341_LED_Pin GPIO_PIN_3
#define ILI9341_LED_GPIO_Port GPIOD
#define ILI9341_SDO_Pin GPIO_PIN_5
#define ILI9341_SDO_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

typedef enum
{
  FOCUS_TRIGGER,
  FOCUS_VERTICAL,
  FOCUS_TIMEBASE,
  FOCUS_COUPLING,
  FOCUS_CALIBRATION
} KnobFocus_t;

typedef enum
{
  OSC_STATE_RUN,          // 连续运行模式
  OSC_STATE_STOP,         // 停止模式 (画面冻结)
  OSC_STATE_SINGLE_WAIT,  // 单次 - 等待边沿信号
  OSC_STATE_SINGLE_RUN,   // 单次 - 正在采集数据 (新增这个状态!)
  OSC_STATE_SINGLE_DONE   // 单次 - 采集完毕等待显示
} OscState_t;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
