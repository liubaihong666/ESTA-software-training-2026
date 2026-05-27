//
// Created by 24367 on 2026/4/6.
//

#include "algo_trigger.h"

int16_t start_idx_ch1 = 0;
int16_t start_idx_ch2 = 0;

/*
 * 函数名：Algo_FindTriggerPoint
 * 功  能：在数据数组中寻找上升沿触发点
 * 参  数：data_buf - 数据源指针
 *        len - 要搜索的长度 (半个 DMA Buffer)
 *        trigger_val - 当前触发电平对应的 ADC 原始值
 * 返回值：返回触发点在数组中的索引。如果没找到，返回 -1。
 */
int16_t Algo_FindTriggerPoint(const uint16_t* data_buf, uint16_t len, uint16_t trigger_val)
{
    uint16_t search_limit = len - 200;
    for (uint16_t i = 1; i < search_limit; i++)
    {
        if (data_buf[i - 1] < trigger_val && data_buf[i] >= trigger_val)
        {
            return (int16_t)i;
        }
    }
    return -1;
}
