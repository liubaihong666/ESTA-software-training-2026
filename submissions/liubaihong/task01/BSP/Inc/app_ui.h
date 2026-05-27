//
// Created by 24367 on 2026/4/10.
//

#ifndef THE_SECOND_TASK_APP_UI_H
#define THE_SECOND_TASK_APP_UI_H

#include "algo_dsp.h"
#include "algo_measure.h"

// 预定义屏幕布局参数
#define WAVE_OFFSET_X  20    // 波形区左上角 X 坐标
#define WAVE_OFFSET_Y  32    // 波形区左上角 Y 坐标
#define WAVE_WIDTH     200   // 10 div * 20 pts
#define WAVE_HEIGHT    256   // 8 div * 32 pts
// 定义频谱图的显示区域
#define SPEC_OFFSET_X  20    // 左边距
#define SPEC_OFFSET_Y  60    // 顶边距 (留出空间写频率参数)
#define SPEC_WIDTH     200   // 总宽度
#define SPEC_HEIGHT    180   // 柱状图最大高度
#define BAR_WIDTH      24    // 每根柱子的宽度
#define BAR_GAP        12    // 柱子之间的间距

extern float g_display_zoom;

void UI_DrawGrid(void);
void UI_DrawWaveform(const uint16_t* data_buf, uint16_t start_idx, uint16_t color, uint8_t channel);
void UI_DrawSpectrum(const HarmonicData_t* data);
void UI_DrawTopInfoBar(uint8_t channel, const WaveParams_t* params, int8_t gear, uint8_t coupling, float phase_diff);
void UI_DrawBottomFocus(int focus_state);
#endif //THE_SECOND_TASK_APP_UI_H