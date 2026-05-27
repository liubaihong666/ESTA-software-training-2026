//
// Created by 24367 on 2026/4/13.
//

#ifndef THE_SECOND_TASK_ALGO_DSP_H
#define THE_SECOND_TASK_ALGO_DSP_H


#include "arm_math.h"

#define FFT_LENGTH 1024

typedef struct
{
    float Fundamental_Freq;
    float Harmonic_Amp[6];
} HarmonicData_t;

void Algo_DSP_Analyze(const uint16_t* adc_buf, uint32_t sample_rate, HarmonicData_t* out_data);
void Algo_DSP_Init(void);

#endif //THE_SECOND_TASK_ALGO_DSP_H