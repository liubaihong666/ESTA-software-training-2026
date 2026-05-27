//
// Created by 24367 on 2026/4/5.
//

#ifndef THE_SECOND_TASK_BSP_ADC_H
#define THE_SECOND_TASK_BSP_ADC_H

#include "main.h"

// 屏幕水平 20点/div，10div，共计 200 个有效点。
// 为了保证 FFT 的点数需求以及波形触发寻找的冗余，我们缓存开大一点 (例如 2048 点)
#define ADC_RAW_BUFFER_SIZE  4096
// 拆解后的单通道有效数据长度依然是 2048
#define ADC_CHANNEL_SIZE     2048

extern volatile uint8_t g_adc_half_ready;
extern volatile uint8_t g_adc_full_ready;
extern volatile OscState_t g_osc_state;
extern uint16_t g_ch1_buffer[ADC_CHANNEL_SIZE];
extern uint16_t g_ch2_buffer[ADC_CHANNEL_SIZE];
void BSP_ADC_Start(void);
void BSP_ADC_Stop(void);

#endif //THE_SECOND_TASK_BSP_ADC_H