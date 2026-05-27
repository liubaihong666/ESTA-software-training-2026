//
// Created by 24367 on 2026/4/6.
//

#ifndef THE_SECOND_TASK_ALGO_TRIGGER_H
#define THE_SECOND_TASK_ALGO_TRIGGER_H

#include "main.h"
extern int16_t start_idx_ch1;
extern int16_t start_idx_ch2;
int16_t Algo_FindTriggerPoint(const uint16_t* data_buf, uint16_t len, uint16_t trigger_val);

#endif //THE_SECOND_TASK_ALGO_TRIGGER_H