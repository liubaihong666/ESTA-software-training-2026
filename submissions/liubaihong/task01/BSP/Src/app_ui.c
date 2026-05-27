//
// Created by 24367 on 2026/4/10.
//

#include "app_ui.h"
#include "ili9341_driver.h"
#include "bsp_adc.h"
#include <stdio.h>

#define WAVE_OFFSET_X  20    // 波形区左上角 X 坐标
#define WAVE_OFFSET_Y  32    // 波形区左上角 Y 坐标
#define WAVE_WIDTH     200   // 10 div * 20 pts
#define WAVE_HEIGHT    256   // 8 div * 32 pts

static uint16_t last_wave_y_ch1[WAVE_WIDTH] = {0};
static uint16_t last_wave_y_ch2[WAVE_WIDTH] = {0};
static uint8_t  is_first_frame_ch1 = 1;
static uint8_t  is_first_frame_ch2 = 1;

/*
 * 功  能：绘制网格
 */
void UI_DrawGrid(void)
{
    ILI9341_fill(WAVE_OFFSET_X, WAVE_OFFSET_Y,WAVE_OFFSET_X + WAVE_WIDTH, WAVE_OFFSET_Y + WAVE_HEIGHT, BLACK);

    for (int i = 0; i <= 8; i++)
    {
        uint16_t y = WAVE_OFFSET_Y + (i * 32);
        ILI9341_draw_line(WAVE_OFFSET_X, y, WAVE_OFFSET_X + WAVE_WIDTH, y, WHITE);
    }

    for (int i = 0; i <= 10; i++)
    {
        uint16_t x = WAVE_OFFSET_X + (i * 20);
        ILI9341_draw_line(x, WAVE_OFFSET_Y, x, WAVE_OFFSET_Y + WAVE_HEIGHT, WHITE);
    }
    ILI9341_draw_rectangle(WAVE_OFFSET_X, WAVE_OFFSET_Y,WAVE_OFFSET_X + WAVE_WIDTH, WAVE_OFFSET_Y + WAVE_HEIGHT, LIGHTGREEN);
}

/*
 * 功  能：在网格上绘制采集到的波形
 * 参  数：data_buf - DMA 采集数组
 * start_idx - 触发算法算出的起始对齐点
 */
void UI_DrawWaveform(const uint16_t* data_buf, uint16_t start_idx, uint16_t color, uint8_t channel)
{
    uint16_t prev_x = WAVE_OFFSET_X;
    uint16_t prev_y = 0;
    uint16_t current_x, current_y;

    uint16_t* p_last_y = (channel == 1) ? last_wave_y_ch1 : last_wave_y_ch2;
    uint8_t* p_is_first = (channel == 1) ? &is_first_frame_ch1 : &is_first_frame_ch2;

    if (!(*p_is_first))
    {
        prev_y = p_last_y[0];
        for (uint16_t x = 1; x < WAVE_WIDTH; x++)
        {
            current_x = WAVE_OFFSET_X + x;
            current_y = p_last_y[x];
            ILI9341_draw_line(prev_x, prev_y, current_x, current_y, BLACK);
            prev_x = current_x;
            prev_y = current_y;
        }
    }

    for (int i = 0; i <= 8; i++)
    {
        uint16_t y = WAVE_OFFSET_Y + (i * 32);
        ILI9341_draw_line(WAVE_OFFSET_X, y, WAVE_OFFSET_X + WAVE_WIDTH - 1, y, WHITE);
    }
    for (int i = 0; i <= 10; i++)
    {
        uint16_t x = WAVE_OFFSET_X + (i * 20);
        ILI9341_draw_line(x, WAVE_OFFSET_Y, x, WAVE_OFFSET_Y + WAVE_HEIGHT - 1, WHITE);
    }

    for (uint16_t x = 0; x < WAVE_WIDTH; x++)
    {
        float array_idx_f = (float)x / g_display_zoom;
        uint16_t idx_int = (uint16_t)array_idx_f;
        float idx_frac = array_idx_f - (float)idx_int;

        if ((start_idx + idx_int + 1) >= ADC_CHANNEL_SIZE) break;

        float y1 = (float)data_buf[start_idx + idx_int];
        float y2 = (float)data_buf[start_idx + idx_int + 1];

        float interpolated_adc = y1 + idx_frac * (y2 - y1);

        uint16_t mapped_h = (uint16_t)((interpolated_adc / 4095.0f) * WAVE_HEIGHT);
        if (mapped_h >= WAVE_HEIGHT) mapped_h = WAVE_HEIGHT - 1;

        current_y = WAVE_OFFSET_Y + (WAVE_HEIGHT - 1 - mapped_h);
        current_x = WAVE_OFFSET_X + x;

        p_last_y[x] = current_y;

        if (x == 0) ILI9341_draw_point(current_x, current_y, color);
        else ILI9341_draw_line(prev_x, prev_y, current_x, current_y, color);

        prev_x = current_x;
        prev_y = current_y;
    }
    *p_is_first = 0;
}

#define SPEC_OFFSET_X  20    // 左边距
#define SPEC_OFFSET_Y  60    // 顶边距 (留出空间写频率参数)
#define SPEC_WIDTH     200   // 总宽度
#define SPEC_HEIGHT    180   // 柱状图最大高度
#define BAR_WIDTH      24    // 每根柱子的宽度
#define BAR_GAP        12    // 柱子之间的间距

/*
 * 功  能：绘制 1~5 次谐波的频谱柱状图
 * 参  数：data - 包含谐波幅值和基波频率的结构体
 * 说  明：假设 BSP_LCD_FillRect 的参数是 (x1, y1, x2, y2, color)
 */
void UI_DrawSpectrum(const HarmonicData_t* data)
{
    ILI9341_clear(BLACK);

    ILI9341_draw_line(SPEC_OFFSET_X, SPEC_OFFSET_Y, SPEC_OFFSET_X, SPEC_OFFSET_Y + SPEC_HEIGHT, WHITE); // Y轴
    ILI9341_draw_line(SPEC_OFFSET_X, SPEC_OFFSET_Y + SPEC_HEIGHT, SPEC_OFFSET_X + SPEC_WIDTH, SPEC_OFFSET_Y + SPEC_HEIGHT, WHITE); // X轴

    char info_str[32];
    sprintf(info_str, "Fund Freq: %lu Hz", (uint32_t)data->Fundamental_Freq);
    ILI9341_draw_string(SPEC_OFFSET_X + 10, SPEC_OFFSET_Y - 20, info_str, LIGHTGREEN);

    float max_amp = 0.001f;
    for (int i = 1; i <= 5; i++)
    {
        if (data->Harmonic_Amp[i] > max_amp) max_amp = data->Harmonic_Amp[i];
    }

    for (int i = 1; i <= 5; i++)
    {
        float current_amp = data->Harmonic_Amp[i];

        uint16_t bar_h = (uint16_t)((current_amp / max_amp) * (SPEC_HEIGHT - 20));
        if (bar_h > SPEC_HEIGHT) bar_h = SPEC_HEIGHT;
        if (bar_h == 0) bar_h = 1;

        uint16_t x_start = SPEC_OFFSET_X + 16 + (i - 1) * (BAR_WIDTH + BAR_GAP);
        uint16_t y_start = SPEC_OFFSET_Y + SPEC_HEIGHT - bar_h;

        uint16_t bar_color = (i == 1) ? YELLOW : CYAN;

        ILI9341_fill(x_start, y_start, x_start + BAR_WIDTH, y_start + bar_h, bar_color);

        char label_str[5];
        sprintf(label_str, "%df0", i);
        ILI9341_draw_string(x_start + 2, SPEC_OFFSET_Y + SPEC_HEIGHT + 4, label_str, WHITE);

        if (i == 1)
        {
            ILI9341_draw_string(x_start, y_start - 16, "100%", YELLOW);
        }
        else if (data->Harmonic_Amp[1] > 0.01f)
        {
            uint8_t percent = (uint8_t)((current_amp / data->Harmonic_Amp[1]) * 100);
            char pct_str[10];
            sprintf(pct_str, "%d%%", percent);
            ILI9341_draw_string(x_start, y_start - 16, pct_str, ORANGE);
        }
    }
}

/*
 * 功  能：在屏幕顶部 0~31 像素区域绘制实时参数
 * 参  数：params - 测量出的波形参数结构体
 * gear   - 当前的垂直灵敏度档位
 * coupling - 0:DC直流, 1:AC交流
 * phase_diff - 两通道相位差 (0~360度)
 */
void UI_DrawTopInfoBar(uint8_t channel, const WaveParams_t* params, int8_t gear, uint8_t coupling, float phase_diff)
{
    ILI9341_fill(0, 0, 240, 31, BLACK);
    uint16_t ch_color = (channel == 1) ? YELLOW : CYAN;
    char ch_str[10];

    if (coupling == 0)
    {
        sprintf(ch_str, "CH%d(DC):", channel);
        ILI9341_draw_string(0, 0, ch_str, ch_color);
    }
    else
    {
        sprintf(ch_str, "CH%d(AC):", channel);
        ILI9341_draw_string(0, 0, ch_str, ORANGE);
    }

    uint16_t str_offset = 64;
    if (gear == 0)      ILI9341_draw_string(str_offset, 0, "1V/div", ch_color);
    else if (gear == 1) ILI9341_draw_string(str_offset, 0, "0.1V/div", ch_color);
    else if (gear == 2) ILI9341_draw_string(str_offset, 0, "10mV/div", ch_color);

    ILI9341_draw_string(136, 0, "Vp:", WHITE);
    ILI9341_float_3_display(160, 0, params->Vpp, LIGHTGREEN);
    ILI9341_draw_string(216, 0, "V", LIGHTGREEN);

    ILI9341_draw_string(0, 16, "F:", WHITE);

    //ILI9341_draw_num(16, 16, (uint32_t)params->Freq, 5, CYAN);
    //ILI9341_draw_string(70, 16, "Hz", CYAN);

    char freq_str[16];
    float f = params->Freq;
    if (f >= 1000.0f)
    {
        uint32_t k_int = (uint32_t)(f / 1000.0f);
        uint32_t k_dec = (uint32_t)((f / 1000.0f - (float)k_int) * 10.0f);
        sprintf(freq_str, "%lu.%lukHz  ", k_int, k_dec);
    }
    else
    {
        sprintf(freq_str, "%luHz     ", (uint32_t)f);
    }
    ILI9341_draw_string(16, 16, freq_str, CYAN);

    ILI9341_draw_string(88, 16, "Ph:", WHITE);
    ILI9341_draw_num(112, 16, (uint32_t)phase_diff, 3, WHITE);
    ILI9341_draw_string(136, 16, "deg", WHITE);
    ILI9341_draw_string(168, 16, "A:", WHITE);
    ILI9341_float_3_display(184, 16, params->Vavg, ORANGE);
    ILI9341_draw_string(224, 16, "V", ORANGE);
}

/*
 * 功  能：在屏幕底部(288~319)绘制当前的旋钮控制焦点
 * 参  数：focus_state - 传入当前的 current_focus
 */
void UI_DrawBottomFocus(int focus_state)
{
    ILI9341_fill(0, 290, 240, 319, BLACK);

    char *focus_str = "";
    uint16_t color = WHITE;

    switch (focus_state)
    {
        case 0:
            focus_str = "[ KNOB: TRIGGER ]  ";
            color = LIGHTGREEN;
            break;
        case 1:
            focus_str = "[ KNOB: VERTICAL ] ";
            color = LIGHTGREEN;
            break;
        case 2:
            focus_str = "[ KNOB: TIMEBASE ] ";
            color = LIGHTGREEN;
            break;
        case 3:
            focus_str = "[ KNOB: COUPLING ] ";
            color = LIGHTGREEN;
            break;
        case 4:
            focus_str = "[ KNOB: CALIBRATE ]";
            color = LIGHTGREEN;
            break;
        default:
            focus_str = "[ KNOB: UNKNOWN ]  ";
            break;
    }

    // 在屏幕正下方居中显示 (X=40, Y=296)
    ILI9341_draw_string(40, 296, focus_str, color);
}
