/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CH1_ADG409_A0_Pin|CH2_IN_ADG408_A1_Pin|CH1_ADG408_A2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ILI9341_SDI_Pin|ILI9341_RS_Pin|ILI9341_CS_Pin|CH2_ADG408_A0_Pin
                          |CH2_ADG408_A1_Pin|CH1_IN_ADG408_A1_Pin|CH1_ADG408_A1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CH2_IN_ADG408_A0_Pin|CH2_ADG408_A2_Pin|ILI9341_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CH2_ADG409_A0_GPIO_Port, CH2_ADG409_A0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, CH1_IN_ADG408_A0_Pin|CH1_ADG409_A1_Pin|CH2_ADG409_A1_Pin|ILI9341_SCK_Pin
                          |CH1_ADG408_A0_Pin|ILI9341_LED_Pin|ILI9341_SDO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CH1_ADG409_A0_Pin CH1_ADG408_A2_Pin */
  GPIO_InitStruct.Pin = CH1_ADG409_A0_Pin|CH1_ADG408_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CH2_IN_ADG408_A1_Pin */
  GPIO_InitStruct.Pin = CH2_IN_ADG408_A1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CH2_IN_ADG408_A1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ILI9341_SDI_Pin ILI9341_RS_Pin ILI9341_CS_Pin CH1_IN_ADG408_A1_Pin */
  GPIO_InitStruct.Pin = ILI9341_SDI_Pin|ILI9341_RS_Pin|ILI9341_CS_Pin|CH1_IN_ADG408_A1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CH2_IN_ADG408_A0_Pin ILI9341_RST_Pin */
  GPIO_InitStruct.Pin = CH2_IN_ADG408_A0_Pin|ILI9341_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : CH2_ADG408_A2_Pin */
  GPIO_InitStruct.Pin = CH2_ADG408_A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CH2_ADG408_A2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_ENCODER_Pin */
  GPIO_InitStruct.Pin = KEY_ENCODER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY_ENCODER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_AUTO_Pin */
  GPIO_InitStruct.Pin = KEY_AUTO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_AUTO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RISING_EDGE_TRIGGERING_Pin */
  GPIO_InitStruct.Pin = RISING_EDGE_TRIGGERING_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RISING_EDGE_TRIGGERING_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_SINGLE_Pin */
  GPIO_InitStruct.Pin = KEY_SINGLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KEY_SINGLE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CH2_ADG409_A0_Pin */
  GPIO_InitStruct.Pin = CH2_ADG409_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CH2_ADG409_A0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_MODE_Pin KEY_CH_TOGGLE_Pin */
  GPIO_InitStruct.Pin = KEY_MODE_Pin|KEY_CH_TOGGLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CH1_IN_ADG408_A0_Pin ILI9341_SCK_Pin ILI9341_LED_Pin ILI9341_SDO_Pin */
  GPIO_InitStruct.Pin = CH1_IN_ADG408_A0_Pin|ILI9341_SCK_Pin|ILI9341_LED_Pin|ILI9341_SDO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : CH1_ADG409_A1_Pin CH2_ADG409_A1_Pin CH1_ADG408_A0_Pin */
  GPIO_InitStruct.Pin = CH1_ADG409_A1_Pin|CH2_ADG409_A1_Pin|CH1_ADG408_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : CH2_ADG408_A0_Pin CH2_ADG408_A1_Pin CH1_ADG408_A1_Pin */
  GPIO_InitStruct.Pin = CH2_ADG408_A0_Pin|CH2_ADG408_A1_Pin|CH1_ADG408_A1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_CLOSE);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
