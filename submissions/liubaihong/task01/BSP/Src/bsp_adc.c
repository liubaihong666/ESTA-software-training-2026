//
// Created by 24367 on 2026/4/5.
//

#include "bsp_adc.h"
#include "adc.h"

__attribute__((used, section(".noncacheable_data"))) uint16_t g_adc_raw_buffer[ADC_RAW_BUFFER_SIZE];
uint16_t g_ch1_buffer[ADC_CHANNEL_SIZE];
uint16_t g_ch2_buffer[ADC_CHANNEL_SIZE];

void BSP_ADC_Start(void)
{
    g_adc_half_ready = 0;
    g_adc_full_ready = 0;
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)g_adc_raw_buffer, ADC_RAW_BUFFER_SIZE);
}

void BSP_ADC_Stop(void)
{
    HAL_ADC_Stop_DMA(&hadc1);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        for (uint16_t i = 0; i < ADC_CHANNEL_SIZE / 2; i++)
        {
            g_ch1_buffer[i] = g_adc_raw_buffer[i * 2];
            g_ch2_buffer[i] = g_adc_raw_buffer[i * 2 + 1];
        }
        g_adc_half_ready = 1;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        for (uint16_t i = ADC_CHANNEL_SIZE / 2; i < ADC_CHANNEL_SIZE; i++)
        {
            g_ch1_buffer[i] = g_adc_raw_buffer[i * 2];
            g_ch2_buffer[i] = g_adc_raw_buffer[i * 2 + 1];
        }
        g_adc_full_ready = 1;
        if (g_osc_state == OSC_STATE_SINGLE_RUN)
        {
            BSP_ADC_Stop();
            g_osc_state = OSC_STATE_SINGLE_DONE;
        }
    }
}