//
// Created by 24367 on 2026/4/13.
//

#include "algo_dsp.h"
#include <math.h>
__attribute__((section(".noncacheable_data"))) float fft_input[FFT_LENGTH];
__attribute__((section(".noncacheable_data"))) float fft_output[FFT_LENGTH];
__attribute__((section(".noncacheable_data"))) float fft_mag[FFT_LENGTH / 2];
__attribute__((section(".noncacheable_data"))) float hanning_window[FFT_LENGTH];
arm_rfft_fast_instance_f32 fft_instance;

void Algo_DSP_Init(void)
{
    arm_rfft_fast_init_f32(&fft_instance, FFT_LENGTH);
    for (int i = 0; i < FFT_LENGTH; i++)
    {
        hanning_window[i] = 0.5f * (1.0f - cosf(2.0f * 3.14159265f * (float)i / (float)(FFT_LENGTH - 1)));
    }
}

/*
 * 功  能：对采集数据进行 FFT，并提取基波和 2~5 次谐波的幅值
 * 参  数：adc_buf     - 原始 ADC 数据
 * sample_rate - 当前的采样率
 * out_data    - 结果存放结构体
 */
void Algo_DSP_Analyze(const uint16_t* adc_buf, uint32_t sample_rate, HarmonicData_t* out_data)
{
    uint32_t sum = 0;
    for (int i = 0; i < FFT_LENGTH; i++) sum += adc_buf[i];
    float avg = (float)sum / FFT_LENGTH;

    for (int i = 0; i < FFT_LENGTH; i++)
    {
        fft_input[i] = ((float)adc_buf[i] - avg) * hanning_window[i];
    }

    arm_rfft_fast_f32(&fft_instance, fft_input, fft_output, 0);
    arm_cmplx_mag_f32(fft_output, fft_mag, FFT_LENGTH / 2);

    float max_val;
    uint32_t max_index;
    arm_max_f32(&fft_mag[3], (FFT_LENGTH / 2) - 3, &max_val, &max_index);
    max_index += 3;

    float freq_resolution = (float)sample_rate / FFT_LENGTH;
    out_data->Fundamental_Freq = (float)max_index * freq_resolution;

    for (int n = 1; n <= 5; n++)
    {
        uint32_t center_idx = max_index * n;

        if (center_idx < (FFT_LENGTH / 2) - 3)
        {
            float local_max = 0.0f;
            for (int j = -2; j <= 2; j++)
            {
                if (fft_mag[center_idx + j] > local_max)
                {
                    local_max = fft_mag[center_idx + j];
                }
            }
            out_data->Harmonic_Amp[n] = local_max;
        }
        else
        {
            out_data->Harmonic_Amp[n] = 0.0f;
        }
    }
}
