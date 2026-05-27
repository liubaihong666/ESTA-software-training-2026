//
// Created by 24367 on 2026/4/6.
//

#ifndef THE_SECOND_TASK_BSP_TIMER_H
#define THE_SECOND_TASK_BSP_TIMER_H

#include "main.h"

void BSP_Timer_SetSampleRate(uint32_t sample_rate_hz);
void BSP_Timer_Start(void);
void BSP_Timer_Stop(void);

#endif //THE_SECOND_TASK_BSP_TIMER_H