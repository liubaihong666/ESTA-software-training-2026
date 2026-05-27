//
// Created by 24367 on 2026/4/15.
//

#include "bsp_encoder.h"
#include "tim.h"

void BSP_Encoder_Init(void)
{
    __HAL_TIM_SET_COUNTER(&htim3, 32768);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

/*
 * 功  能：获取自上次读取以来的旋转步数和方向
 * 返回值：>0 表示正转，<0 表示反转，0 表示未旋转
 */
int16_t BSP_Encoder_GetDelta(void)
{
    static uint16_t last_count = 32768;
    uint16_t current_count = __HAL_TIM_GET_COUNTER(&htim3);

    if (current_count == last_count)
    {
        return 0;
    }

    int16_t delta = (int16_t)( ((int16_t)(current_count - last_count)) / 4 );//四通频

    if (delta != 0)
    {
        last_count += delta * 4;
    }
    return delta;
}
