//
// Created by 24367 on 2026/4/6.
//

#include "bsp_dac.h"
#include "dac.h"
void BSP_DAC_Init(void)
{
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

/*
 * 函数名：BSP_DAC_SetVoltage
 * 功  能：设置 DAC 输出的具体电压
 * 参  数：target_voltage_V - 目标电压值
 * ref_voltage_V - 芯片的参考电压
 */
void BSP_DAC_SetVoltage(float target_voltage_V, float ref_voltage_V)
{
    if (target_voltage_V > ref_voltage_V) target_voltage_V = ref_voltage_V;
    if (target_voltage_V < 0.0f) target_voltage_V = 0.0f;
    uint32_t dac_val = (uint32_t)((target_voltage_V / ref_voltage_V) * 4095.0f);
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_val);
}