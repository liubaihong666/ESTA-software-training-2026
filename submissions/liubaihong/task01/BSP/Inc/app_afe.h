//
// Created by 24367 on 2026/4/17.
//

#ifndef THE_SECOND_TASK_APP_AFE_H
#define THE_SECOND_TASK_APP_AFE_H

#include "bsp_adc.h"

void AFE_SetGear_Dual(int8_t gear);
void AFE_SetGear_Single(uint8_t channel, int8_t gear);
void AFE_SetCoupling_Single(uint8_t channel, uint8_t is_ac);
void AFE_SetCoupling_Dual(uint8_t is_ac);
void AFE_EnterCalibrationMode(uint8_t channel);
#endif //THE_SECOND_TASK_APP_AFE_H