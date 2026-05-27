//
// Created by 24367 on 2026/4/17.
//

#include "app_afe.h"


void AFE_SetCoupling_Single(uint8_t channel, uint8_t is_ac)
{
    BSP_ADC_Stop();

    // 根据 is_ac 状态决定 A0 的电平 (AC: 0, DC: 1)
    GPIO_PinState a0_state = is_ac ? GPIO_PIN_RESET : GPIO_PIN_SET;

    if (channel == 1)
    {
        HAL_GPIO_WritePin(CH1_IN_ADG408_A0_GPIO_Port, CH1_IN_ADG408_A0_Pin, a0_state);
        HAL_GPIO_WritePin(CH1_IN_ADG408_A1_GPIO_Port, CH1_IN_ADG408_A1_Pin, GPIO_PIN_RESET);
    }
    else if (channel == 2)
    {
        HAL_GPIO_WritePin(CH2_IN_ADG408_A0_GPIO_Port, CH2_IN_ADG408_A0_Pin, a0_state);
        HAL_GPIO_WritePin(CH2_IN_ADG408_A1_GPIO_Port, CH2_IN_ADG408_A1_Pin, GPIO_PIN_RESET);
    }

    HAL_Delay(10);
    BSP_ADC_Start();
}

/*
 * 功  能：同步切换双通道交直流耦合
 */
void AFE_SetCoupling_Dual(uint8_t is_ac)
{
    AFE_SetCoupling_Single(1, is_ac);
    AFE_SetCoupling_Single(2, is_ac);
}

/*
 * 功  能：将输入端 ADG408 切入内部校正信号通道 (010)
 * 说  明：供一键自校准程序调用
 */
void AFE_EnterCalibrationMode(uint8_t channel)
{
    BSP_ADC_Stop();
    if (channel == 1)
    {
        HAL_GPIO_WritePin(CH1_IN_ADG408_A0_GPIO_Port, CH1_IN_ADG408_A0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(CH1_IN_ADG408_A1_GPIO_Port, CH1_IN_ADG408_A1_Pin, GPIO_PIN_SET);
    }
    else if (channel == 2)
    {
        HAL_GPIO_WritePin(CH2_IN_ADG408_A0_GPIO_Port, CH2_IN_ADG408_A0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(CH2_IN_ADG408_A1_GPIO_Port, CH2_IN_ADG408_A1_Pin, GPIO_PIN_SET);
    }
    HAL_Delay(50);
    BSP_ADC_Start();
}

/*
 * 功  能：同步切换双通道档位 (用于开机初始化或双通道联动调节)
 */
void AFE_SetGear_Dual(int8_t gear)
{
    AFE_SetGear_Single(1, gear);
    AFE_SetGear_Single(2, gear);
}

/**
 * @brief  核心底层：独立设置指定通道的硬件增益 (支持双 CD4052 级联)
 * @param  channel: 1 代表 CH1, 2 代表 CH2
 * @param  gear:    目标档位 (0:1V/div, 1:0.1V/div, 2:0.01V/div)
 */
void AFE_SetGear_Single(uint8_t channel, int8_t gear)
{
    BSP_ADC_Stop();
    GPIO_PinState adg408_a0, adg408_a1, adg408_a2;
    GPIO_PinState adg409_a0, adg409_a1;

    switch(gear)
    {
        case 0:
            adg408_a2 = GPIO_PIN_RESET;
            adg408_a1 = GPIO_PIN_SET;
            adg408_a0 = GPIO_PIN_SET;

            adg409_a1 = GPIO_PIN_RESET;
            adg409_a0 = GPIO_PIN_RESET;
            break;

        case 1:
            adg408_a2 = GPIO_PIN_RESET;
            adg408_a1 = GPIO_PIN_RESET;
            adg408_a0 = GPIO_PIN_RESET;

            adg409_a1 = GPIO_PIN_RESET;
            adg409_a0 = GPIO_PIN_RESET;
            break;

        case 2:
            adg408_a2 = GPIO_PIN_RESET;
            adg408_a1 = GPIO_PIN_RESET;
            adg408_a0 = GPIO_PIN_RESET;

            adg409_a1 = GPIO_PIN_RESET;
            adg409_a0 = GPIO_PIN_SET;
            break;

        default:
            BSP_ADC_Start();
            return;
    }

    if (channel == 1)
    {
        HAL_GPIO_WritePin(CH1_ADG408_A0_GPIO_Port, CH1_ADG408_A0_Pin, adg408_a0);
        HAL_GPIO_WritePin(CH1_ADG408_A1_GPIO_Port, CH1_ADG408_A1_Pin, adg408_a1);
        HAL_GPIO_WritePin(CH1_ADG408_A2_GPIO_Port, CH1_ADG408_A2_Pin, adg408_a2);
        HAL_GPIO_WritePin(CH1_ADG409_A0_GPIO_Port, CH1_ADG409_A0_Pin, adg409_a0);
        HAL_GPIO_WritePin(CH1_ADG409_A1_GPIO_Port, CH1_ADG409_A1_Pin, adg409_a1);
    }
    else if (channel == 2)
    {
        HAL_GPIO_WritePin(CH2_ADG408_A0_GPIO_Port, CH2_ADG408_A0_Pin, adg408_a0);
        HAL_GPIO_WritePin(CH2_ADG408_A1_GPIO_Port, CH2_ADG408_A1_Pin, adg408_a1);
        HAL_GPIO_WritePin(CH2_ADG408_A2_GPIO_Port, CH2_ADG408_A2_Pin, adg408_a2);
        HAL_GPIO_WritePin(CH2_ADG409_A0_GPIO_Port, CH2_ADG409_A0_Pin, adg409_a0);
        HAL_GPIO_WritePin(CH2_ADG409_A1_GPIO_Port, CH2_ADG409_A1_Pin, adg409_a1);
    }
    HAL_Delay(2);
    BSP_ADC_Start();
}
