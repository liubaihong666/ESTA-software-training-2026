//
// Created by 24367 on 2026/4/10.
//

#include "algo_measure.h"
#include "bsp_adc.h"

const float Hardware_Scale_Factor[3] = {10.0f, 1.0f, 0.1f};


/*
 * 函数名：Algo_Measure_Basic
 * 功  能：计算基础波形参数 (峰峰值、均值、频率)
 */
void Algo_Measure_Basic(const uint16_t* data_buf, uint16_t len, uint32_t sample_rate_hz, WaveParams_t* out_params, float v_comp, float f_comp)
{
    uint16_t max_val = 0;
    uint16_t min_val = 4095;
    uint32_t sum = 0;

    for (uint16_t i = 0; i < len; i++)
    {
        if (data_buf[i] > max_val) max_val = data_buf[i];
        if (data_buf[i] < min_val) min_val = data_buf[i];
        sum += data_buf[i];
    }

    float v_ref = 3.3f;
    out_params->Vmax = ((float)max_val / 4095.0f) * v_ref;
    out_params->Vmin = ((float)min_val / 4095.0f) * v_ref;
    out_params->Vpp  = out_params->Vmax - out_params->Vmin;
    out_params->Vavg = (((float)sum / (float)len) / 4095.0f) * v_ref;

    //频率测量
    uint16_t avg_val = sum / len;
    //设定阈值
    uint16_t hysteresis = (max_val - min_val) / 20;
    if (hysteresis < 5) hysteresis = 5;

    uint16_t threshold_high = avg_val + hysteresis;
    uint16_t threshold_low  = avg_val - hysteresis;

    float first_edge_exact = -1.0f;
    float last_edge_exact = -1.0f;
    uint16_t edge_count = 0;

    uint8_t is_ready_for_edge = 1;

    for (uint16_t i = 1; i < len; i++)
    {
        if (data_buf[i] < threshold_low)
        {
            is_ready_for_edge = 1;
        }
        else if (is_ready_for_edge && data_buf[i] >= threshold_high)
        {
            is_ready_for_edge = 0;

            uint16_t idx_after = i;
            while(idx_after > 1 && data_buf[idx_after - 1] >= avg_val)
            {
                idx_after--;
            }
            uint16_t idx_before = idx_after - 1;

            float y1 = (float)data_buf[idx_before];
            float y2 = (float)data_buf[idx_after];

            float fraction = ((float)avg_val - y1) / (y2 - y1);
            float exact_idx = (float)idx_before + fraction;

            if (edge_count == 0)
            {
                first_edge_exact = exact_idx;
            }
            last_edge_exact = exact_idx;
            edge_count++;
        }
    }

    if (edge_count > 1)
    {
        float delta_points = last_edge_exact - first_edge_exact;
        out_params->Freq = ((float)(edge_count - 1) * (float)sample_rate_hz * f_comp) / delta_points;
    }
    else
    {
        out_params->Freq = 0.0f; // 频率太低或直流
    }

    // 获取当前垂直档位
    int8_t gear = 1;
    if (data_buf == g_ch1_buffer)
    {
        gear = g_ch1_gear;
    } else if (data_buf == g_ch2_buffer)
    {
        gear = g_ch2_gear;
    }

    float scale = Hardware_Scale_Factor[gear];

    // 计算真实电压
    float adc_vmax = ((float)max_val / 4095.0f) * 3.3f;
    out_params->Vmax = (adc_vmax - 1.65f) * scale * v_comp;
    out_params->Vmin = ((((float)min_val / 4095.0f) * 3.3f) - 1.65f) * scale * v_comp;
    out_params->Vpp = (((float)(max_val - min_val) / 4095.0f) * 3.3f) * scale * v_comp;
}


/*
 * 功  能：寻找一段波形中所有的精确过零上升沿 (多点寻峰)
 * 参  数：buf - 数据源, len - 长度, edges_out - 保存边沿索引的数组, max_edges - 最大保存数量
 * 返回值：实际找到的边沿数量
 */
static int Find_All_Exact_Edges(const uint16_t* buf, uint16_t len, float* edges_out, int max_edges)
{
    uint16_t max_val = 0, min_val = 4095;
    uint32_t sum = 0;
    for (uint16_t i = 0; i < len; i++)
    {
        if (buf[i] > max_val) max_val = buf[i];
        if (buf[i] < min_val) min_val = buf[i];
        sum += buf[i];
    }
    uint16_t avg = sum / len;

    uint16_t hyst = (max_val - min_val) / 20;
    if (hyst < 5) hyst = 5;

    uint16_t th_high = avg + hyst;
    uint16_t th_low  = avg - hyst;

    int edge_cnt = 0;
    uint8_t ready = (buf[0] < th_low) ? 1 : 0;

    for (uint16_t i = 1; i < len; i++)
    {
        if (buf[i] < th_low)
        {
            ready = 1;
        }
        else if (ready && buf[i] >= th_high)
        {
            uint16_t idx_after = i;
            while(idx_after > 1 && buf[idx_after - 1] >= avg) idx_after--;
            uint16_t idx_before = idx_after - 1;

            float y1 = (float)buf[idx_before];
            float y2 = (float)buf[idx_after];

            float exact_idx = (float)idx_before + ((float)avg - y1) / (y2 - y1);

            if (edge_cnt < max_edges) {
                edges_out[edge_cnt++] = exact_idx;
            }
            ready = 0;
        }
    }
    return edge_cnt;
}
static float my_fabs(float x) { return (x < 0.0f) ? -x : x; }
#define SYSTEM_PHASE_ERROR_OFFSET  0.0f
/*
 * 功  能：高精度计算两路信号的相位差
 * 暂时没研究成怎么成功测出相位差
 */
float Algo_Measure_PhaseDiff(const uint16_t* buf1, const uint16_t* buf2, uint16_t len, uint32_t sample_rate, float freq_ch1)
{
    if (freq_ch1 < 5.0f) return 0.0f;

    #define MAX_EDGES 100
    float edges1[MAX_EDGES];
    float edges2[MAX_EDGES];

    // 1. 抓取双通道所有的边沿
    int cnt1 = Find_All_Exact_Edges(buf1, len, edges1, MAX_EDGES);
    int cnt2 = Find_All_Exact_Edges(buf2, len, edges2, MAX_EDGES);

    if (cnt1 == 0 || cnt2 == 0) return 0.0f;

    float T_points = (float)sample_rate / freq_ch1;
    float sum_phase = 0.0f;
    int valid_pairs = 0;

    for (int i = 0; i < cnt1; i++)
    {
        float e1 = edges1[i];
        float min_dist = 999999.0f;

        for (int j = 0; j < cnt2; j++)
        {
            float dist = edges2[j] - e1;
            if (my_fabs(dist) < my_fabs(min_dist))
            {
                min_dist = dist;
            }
        }

        if (my_fabs(min_dist) < (T_points * 0.6f))
        {
            float phase = (min_dist / T_points) * 360.0f;
            sum_phase += phase;
            valid_pairs++;
        }
    }

    if (valid_pairs == 0) return 0.0f;

    float avg_phase_diff = sum_phase / (float)valid_pairs;

    avg_phase_diff -= SYSTEM_PHASE_ERROR_OFFSET;

    while (avg_phase_diff > 180.0f)  avg_phase_diff -= 360.0f;
    while (avg_phase_diff < -180.0f) avg_phase_diff += 360.0f;

    float final_result = my_fabs(avg_phase_diff);

    return final_result;
}