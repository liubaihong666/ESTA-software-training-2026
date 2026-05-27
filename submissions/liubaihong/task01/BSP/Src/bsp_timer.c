//
// Created by 24367 on 2026/4/6.
//
#include "bsp_timer.h"
#include "tim.h"

/*
 * 函数名：BSP_Timer_SetSampleRate
 * 功  能：动态设置 ADC 的采样率
 * 参  数：sample_rate_hz - 期望的采样频率
 */
void BSP_Timer_SetSampleRate(uint32_t sample_rate_hz)
{
    uint32_t tim_clk = 240000000;
    uint32_t arr_value = (tim_clk / sample_rate_hz) - 1;

    BSP_Timer_Stop();

    __HAL_TIM_SET_AUTORELOAD(&htim2, arr_value);
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    BSP_Timer_Start();
}

void BSP_Timer_Start(void)
{
    HAL_TIM_Base_Start(&htim2);
}

void BSP_Timer_Stop(void)
{
    HAL_TIM_Base_Stop(&htim2);
}