//
// Created by 24367 on 2026/4/15.
//

#ifndef THE_SECOND_TASK_APP_AUTO_H
#define THE_SECOND_TASK_APP_AUTO_H

#include "main.h"

#define V_REF 3.3f
#define the_number_of_wave_T 5.0f

extern int8_t g_ch1_gear;
extern int8_t g_ch2_gear;
extern uint16_t trigger_val_ch1;
extern uint16_t trigger_val_ch2;
extern uint32_t current_sample_rate;

extern float g_display_zoom;
extern float g_ch1_volt_comp;
extern float g_ch2_volt_comp;
extern float g_ch1_freq_comp;
extern float g_ch2_freq_comp;

void AUTO_AdjustVertical(uint8_t channel);
void AUTO_AdjustTrigger(uint8_t channel);
void AUTO_AdjustTimebase(uint8_t channel);
void AUTO_Execute_Channel(uint8_t channel);
void AUTO_Execute_All(void);
void Wait_For_New_ADC_Frame(void);
int8_t AUTO_GetCurrentGear(uint8_t channel);

#endif //THE_SECOND_TASK_APP_AUTO_H