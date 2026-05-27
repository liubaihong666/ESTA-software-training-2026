//
// Created by 24367 on 2026/4/15.
//

#include "app_auto.h"
#include "app_afe.h"
#include "bsp_timer.h"
#include "algo_measure.h"
#include "bsp_dac.h"

void Wait_For_New_ADC_Frame(void)
{
    g_adc_full_ready = 0;
    uint32_t timeout = HAL_GetTick();
    while(g_adc_full_ready == 0)
    {
        if(HAL_GetTick() - timeout > 500) break;
    }
    g_adc_full_ready = 0;
}

void AUTO_AdjustTrigger(uint8_t channel)
{
    WaveParams_t temp;
    uint16_t* target_buf = (channel == 1) ? g_ch1_buffer : g_ch2_buffer;
    float v_comp = (channel == 1) ? g_ch1_volt_comp : g_ch2_volt_comp;
    float f_comp = (channel == 1) ? g_ch1_freq_comp : g_ch2_freq_comp;

    Algo_Measure_Basic(target_buf, ADC_CHANNEL_SIZE, current_sample_rate, &temp, v_comp, f_comp);
    uint16_t new_trigger_val = (uint16_t)((temp.Vavg / V_REF) * 4095.0f);

    if (channel == 1)
    {
        trigger_val_ch1 = new_trigger_val;
        BSP_DAC_SetVoltage(temp.Vavg, V_REF);
    }
    else
    {
        trigger_val_ch2 = new_trigger_val;
        BSP_DAC_SetVoltage(temp.Vavg, V_REF);
    }
}

void AUTO_AdjustVertical(uint8_t channel)
{
    WaveParams_t temp;
    uint16_t* target_buf = (channel == 1) ? g_ch1_buffer : g_ch2_buffer;
    int8_t* current_gear = (channel == 1) ? &g_ch1_gear : &g_ch2_gear;
    float v_comp         = (channel == 1) ? g_ch1_volt_comp : g_ch2_volt_comp;
    float f_comp         = (channel == 1) ? g_ch1_freq_comp : g_ch2_freq_comp;

    Algo_Measure_Basic(target_buf, ADC_CHANNEL_SIZE, current_sample_rate, &temp, v_comp, f_comp);
    uint8_t gear_changed = 0;

    if (temp.Vmax > 3.1f || temp.Vmin < 0.2f)
    {
        if (*current_gear > 0)
        {
            (*current_gear)--;
            gear_changed = 1;
        }
    }
    else if (temp.Vpp < 0.8f && temp.Vpp > 0.05f)
    {
        if (*current_gear < 2)
        {
            (*current_gear)++;
            gear_changed = 1;
        }
    }
    if (gear_changed)
    {
        AFE_SetGear_Single(channel, *current_gear);
    }
}

void AUTO_AdjustTimebase(uint8_t channel)
{
    WaveParams_t temp;
    uint16_t* target_buf = (channel == 1) ? g_ch1_buffer : g_ch2_buffer;
    float v_comp = (channel == 1) ? g_ch1_volt_comp : g_ch2_volt_comp;
    float f_comp = (channel == 1) ? g_ch1_freq_comp : g_ch2_freq_comp;

    Algo_Measure_Basic(target_buf, ADC_CHANNEL_SIZE, current_sample_rate, &temp, v_comp, f_comp);

    float freq = temp.Freq;
    if (freq < 5.0f) return;

    float T = 1.0f / freq;
    float target_screen_time = the_number_of_wave_T * T;
    uint32_t new_sample_rate = (uint32_t)(200.0f / target_screen_time);

    uint32_t HARDWARE_MAX_SR = 1000000;

    if (new_sample_rate > HARDWARE_MAX_SR)
    {
        BSP_Timer_SetSampleRate(HARDWARE_MAX_SR);
        current_sample_rate = HARDWARE_MAX_SR;
        g_display_zoom = (float)new_sample_rate / (float)HARDWARE_MAX_SR;
    }
    else
    {
        if (new_sample_rate < 1000) new_sample_rate = 1000;
        BSP_Timer_SetSampleRate(new_sample_rate);
        current_sample_rate = new_sample_rate;
        g_display_zoom = 1.0f;
    }
}

void AUTO_Execute_Channel(uint8_t channel)
{
    AUTO_AdjustVertical(channel);
    Wait_For_New_ADC_Frame();
    AUTO_AdjustTrigger(channel);
}

void AUTO_Execute_All(void)
{
    AUTO_AdjustVertical(1);
    AUTO_AdjustVertical(2);

    Wait_For_New_ADC_Frame();

    AUTO_AdjustTrigger(1);
    AUTO_AdjustTrigger(2);

    AUTO_AdjustTimebase(1);
}

int8_t AUTO_GetCurrentGear(uint8_t channel)
{
    return (channel == 1) ? g_ch1_gear : g_ch2_gear;
}