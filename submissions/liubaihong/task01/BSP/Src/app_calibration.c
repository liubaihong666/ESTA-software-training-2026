//
// Created by 24367 on 2026/4/17.
//

#include "app_calibration.h"
#include "algo_measure.h"
#include "bsp_adc.h"
#include "tim.h"
#include "app_auto.h"
#include "app_afe.h"

/*
 * 功  能：对指定通道执行一键自校准闭环
 * 参  数：channel - 1 代表 CH1, 2 代表 CH2
 */
void AUTO_Execute_Calibration_Independent(uint8_t channel)
{
    Wait_For_New_ADC_Frame();

    WaveParams_t cal_params;
    uint16_t* target_buf = (channel == 1) ? g_ch1_buffer : g_ch2_buffer;
    float* target_v_comp = (channel == 1) ? &g_ch1_volt_comp : &g_ch2_volt_comp;
    float* target_f_comp = (channel == 1) ? &g_ch1_freq_comp : &g_ch2_freq_comp;

    Algo_Measure_Basic(target_buf, ADC_CHANNEL_SIZE, current_sample_rate, &cal_params, 1.0f, 1.0f);

    if (cal_params.Vpp > 0.1f) *target_v_comp = STANDARD_CAL_VPP / cal_params.Vpp;
    if (cal_params.Freq > 10.0f) *target_f_comp = STANDARD_FREQ_HZ / cal_params.Freq;
}
