//
// Created by 24367 on 2026/4/17.
//

#ifndef THE_SECOND_TASK_APP_CALIBRATION_H
#define THE_SECOND_TASK_APP_CALIBRATION_H

#include "main.h"

extern float g_voltage_comp_factor;
extern float g_frequency_comp_factor;
extern uint32_t current_sample_rate;
extern uint8_t g_ch1_coupling;
extern uint8_t g_ch2_coupling;
// 假设硬件板子上生成的标准校正信号真实幅度是 1.0V (需与硬件队友确认)
#define STANDARD_CAL_VPP  4.0f
#define STANDARD_FREQ_HZ  1000.0f

void AUTO_Execute_Calibration_Independent(uint8_t channel);

#endif //THE_SECOND_TASK_APP_CALIBRATION_H