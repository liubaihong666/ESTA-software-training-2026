//
// Created by 24367 on 2026/4/10.
//

#ifndef THE_SECOND_TASK_ALGO_MEASURE_H
#define THE_SECOND_TASK_ALGO_MEASURE_H

#include "main.h"


typedef struct
{
    float Vmax;
    float Vmin;
    float Vpp;
    float Vavg;
    float Freq;
} WaveParams_t;
extern const float Hardware_Scale_Factor[3];

extern float g_voltage_comp_factor;
extern float g_frequency_comp_factor;
extern WaveParams_t g_out_put1;
extern WaveParams_t g_out_put2;
extern int8_t AUTO_GetCurrentGear(uint8_t channel);
extern float g_ch1_volt_comp;
extern float g_ch2_volt_comp;
extern float g_ch1_freq_comp;
extern float g_ch2_freq_comp;
extern int8_t g_ch1_gear;
extern int8_t g_ch2_gear;

void Algo_Measure_Basic(const uint16_t* data_buf, uint16_t len, uint32_t sample_rate_hz, WaveParams_t* out_params, float v_comp, float f_comp);
float Algo_Measure_PhaseDiff(const uint16_t* buf1,const uint16_t* buf2, uint16_t len, uint32_t sample_rate, float freq_ch1);

#endif //THE_SECOND_TASK_ALGO_MEASURE_H