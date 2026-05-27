/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_ui.h"
#include "algo_measure.h"
#include "ili9341_driver.h"
#include "bsp_adc.h"
#include "bsp_dac.h"
#include "bsp_timer.h"
#include "algo_trigger.h"
#include "bsp_encoder.h"
#include "app_calibration.h"
#include "app_auto.h"
#include "app_afe.h"
#include "algo_dsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t g_adc_half_ready = 0;
volatile uint8_t g_adc_full_ready = 0;
volatile uint8_t g_ui_display_channel = 1;
float g_display_zoom = 1.0f;
int8_t g_ch1_gear = 1;
int8_t g_ch2_gear = 1;
uint8_t g_ch1_coupling = 0;
uint8_t g_ch2_coupling = 0;

WaveParams_t g_out_put1 = {0};
WaveParams_t g_out_put2 = {0};
volatile OscState_t g_osc_state = OSC_STATE_RUN;

float g_ch1_volt_comp = 1.0f;
float g_ch2_volt_comp = 0.5f;
float g_ch1_freq_comp = 1.0f;
float g_ch2_freq_comp = 1.0f;

uint32_t current_sample_rate = 1000000;

uint16_t trigger_val_ch1 = 2048;
uint16_t trigger_val_ch2 = 2048;

uint8_t display_mode = 0;
uint8_t auto_flag = 0;
uint8_t cal_flag = 0;
uint8_t is_in_cal_mode = 0;
volatile KnobFocus_t current_focus = FOCUS_TRIGGER;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_DAC1_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_init(BLACK);
  BSP_Timer_SetSampleRate(current_sample_rate);
  BSP_Timer_Start();
  BSP_DAC_Init();
  BSP_DAC_SetVoltage(1.65f, 3.3f);
  BSP_Encoder_Init();
  BSP_ADC_Start();

  Algo_DSP_Init();

  AFE_SetGear_Dual(0);
  AFE_SetCoupling_Dual(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //旋钮板块
    int16_t knob_delta = BSP_Encoder_GetDelta();
    if (knob_delta != 0)
    {
      switch (current_focus)
      {
        case FOCUS_TRIGGER:
        {
          int32_t temp_trig = (g_ui_display_channel == 1) ? trigger_val_ch1 : trigger_val_ch2;
          temp_trig += knob_delta * 50;
          if (temp_trig > 4095) temp_trig = 4095;
          if (temp_trig < 0) temp_trig = 0;

          if (g_ui_display_channel == 1)
          {
            trigger_val_ch1 = (uint16_t)temp_trig;
            BSP_DAC_SetVoltage(((float)trigger_val_ch1/4095.0f)*3.3f, 3.3f);
          }
          else
          {
            trigger_val_ch2 = (uint16_t)temp_trig;
            BSP_DAC_SetVoltage(((float)trigger_val_ch2/4095.0f)*3.3f, 3.3f);
          }
          break;
        }


        case FOCUS_VERTICAL:
        {
          int8_t* p_gear = (g_ui_display_channel == 1) ? &g_ch1_gear : &g_ch2_gear;
          if (knob_delta > 0 && *p_gear < 2) (*p_gear)++;
          if (knob_delta < 0 && *p_gear > 0) (*p_gear)--;
          AFE_SetGear_Single(g_ui_display_channel, *p_gear);
          break;
        }

        case FOCUS_TIMEBASE:
        {
          uint32_t virtual_sr = (uint32_t)(current_sample_rate * g_display_zoom);

          if (virtual_sr >= 1000000)     virtual_sr += knob_delta * 200000;
          else if (virtual_sr >= 100000) virtual_sr += knob_delta * 20000;
          else if (virtual_sr >= 10000)  virtual_sr += knob_delta * 2000;
          else if (virtual_sr >= 1000)   virtual_sr += knob_delta * 200;
          else                           virtual_sr += knob_delta * 20;

          if ((int32_t)virtual_sr < 10) virtual_sr = 10;
          if (virtual_sr > 10000000) virtual_sr = 10000000;

          uint32_t HARDWARE_MAX_SR = 1000000;

          if (virtual_sr > HARDWARE_MAX_SR)
          {
            current_sample_rate = HARDWARE_MAX_SR;
            g_display_zoom = (float)virtual_sr / (float)HARDWARE_MAX_SR;
          }
          else
          {
            current_sample_rate = virtual_sr;
            g_display_zoom = 1.0f;
          }

          BSP_Timer_SetSampleRate(current_sample_rate);
          break;
        }

        case FOCUS_COUPLING:
        {
          uint8_t* p_coup = (g_ui_display_channel == 1) ? &g_ch1_coupling : &g_ch2_coupling;
          *p_coup = !(*p_coup);

          AFE_SetCoupling_Single(g_ui_display_channel, *p_coup);
          break;
        }

        case FOCUS_CALIBRATION:
        {
          ILI9341_fill(0, 0, 240, 31, BLACK);
          is_in_cal_mode = !is_in_cal_mode;

          if (is_in_cal_mode)
          {
            ILI9341_draw_string(30, 8, "CAL MODE ON...", RED);

            int8_t* p_gear = (g_ui_display_channel == 1) ? &g_ch1_gear : &g_ch2_gear;
            *p_gear = 0;
            AFE_SetGear_Single(g_ui_display_channel, 0);

            AFE_EnterCalibrationMode(g_ui_display_channel);
            HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
            cal_flag = 1;
          }
          else
          {
            ILI9341_draw_string(30, 8, "CAL MODE OFF...", WHITE);
            HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
            uint8_t prev_coup = (g_ui_display_channel == 1) ? g_ch1_coupling : g_ch2_coupling;
            AFE_SetCoupling_Single(g_ui_display_channel, prev_coup);
          }
          g_adc_full_ready = 0;
          break;
        }
      }
    }

    //单次触发板块
    if (g_osc_state == OSC_STATE_SINGLE_DONE)
    {
      UI_DrawGrid();
      g_adc_full_ready = 0;
      start_idx_ch1 = Algo_FindTriggerPoint(g_ch1_buffer, ADC_CHANNEL_SIZE, trigger_val_ch1);
      start_idx_ch2 = Algo_FindTriggerPoint(g_ch2_buffer, ADC_CHANNEL_SIZE, trigger_val_ch2);
      if (start_idx_ch1 != -1 && start_idx_ch2 != -1)
      {
        float phase_diff = Algo_Measure_PhaseDiff(g_ch1_buffer, g_ch2_buffer, ADC_CHANNEL_SIZE, current_sample_rate, g_out_put1.Freq);
        if (g_ui_display_channel == 1)UI_DrawTopInfoBar(1, &g_out_put1, g_ch1_gear, g_ch1_coupling, phase_diff);
        else if (g_ui_display_channel == 2) UI_DrawTopInfoBar(2, &g_out_put2, g_ch2_gear, g_ch2_coupling, phase_diff);

        UI_DrawWaveform(g_ch1_buffer, start_idx_ch1, YELLOW, 1);
        //UI_DrawWaveform(g_ch2_buffer, start_idx_ch2, CYAN, 2);
      }
      g_osc_state = OSC_STATE_STOP;
    }

    //连续采集显示
    if (g_adc_full_ready == 1 && g_osc_state == OSC_STATE_RUN)
    {
      BSP_ADC_Stop();
      g_adc_full_ready = 0;
      if (display_mode == 0)
      {
        start_idx_ch1 = Algo_FindTriggerPoint(g_ch1_buffer, ADC_CHANNEL_SIZE, trigger_val_ch1);
        if (start_idx_ch1 == -1)start_idx_ch1 = 0;

        Algo_Measure_Basic(g_ch1_buffer, ADC_CHANNEL_SIZE, current_sample_rate, &g_out_put1, g_ch1_volt_comp, g_ch1_freq_comp);
        Algo_Measure_Basic(g_ch2_buffer, ADC_CHANNEL_SIZE, current_sample_rate, &g_out_put2, g_ch2_volt_comp, g_ch2_freq_comp);
        float phase_diff = Algo_Measure_PhaseDiff(g_ch1_buffer, g_ch2_buffer, ADC_CHANNEL_SIZE, current_sample_rate, g_out_put1.Freq);

        if (start_idx_ch1 != -1 && start_idx_ch2 != -1)
        {
          if (g_ui_display_channel == 1)UI_DrawTopInfoBar(1, &g_out_put1, g_ch1_gear, g_ch1_coupling, phase_diff);
          else if (g_ui_display_channel == 2) UI_DrawTopInfoBar(2, &g_out_put2, g_ch2_gear, g_ch2_coupling, phase_diff);

          UI_DrawWaveform(g_ch1_buffer, start_idx_ch1, YELLOW, 1);
          //UI_DrawWaveform(g_ch2_buffer, start_idx_ch2, CYAN, 2);

          UI_DrawBottomFocus((int)current_focus);
        }
      }
      else if (display_mode == 1)
      {
        HarmonicData_t fft_result;
        Algo_DSP_Analyze(g_ch1_buffer, current_sample_rate, &fft_result);
        UI_DrawSpectrum(&fft_result);
        HAL_Delay(500);
      }
      BSP_ADC_Start();
    }


    if (auto_flag == 1)
    {
      auto_flag = 0;
      AUTO_Execute_All();
    }

    if (cal_flag == 1)
    {
      cal_flag = 0;
      AUTO_Execute_Calibration_Independent(g_ui_display_channel);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  uint32_t current_time = HAL_GetTick();

  if (GPIO_Pin == RISING_EDGE_TRIGGERING_Pin)
  {
    if (g_osc_state == OSC_STATE_SINGLE_WAIT)
    {
      BSP_ADC_Start();
      g_osc_state = OSC_STATE_SINGLE_RUN;
    }
  }
  if (GPIO_Pin == KEY_SINGLE_Pin)
  {
    static uint32_t last_single_time = 0;
    if (current_time - last_single_time > 200)
    {
      last_single_time = current_time;
      BSP_ADC_Stop();
      g_osc_state = OSC_STATE_SINGLE_WAIT;
    }
  }
  if (GPIO_Pin == KEY_MODE_Pin)
  {
    static uint32_t last_mode_time = 0;
    if (current_time - last_mode_time > 200)
    {
      last_mode_time = current_time;
      display_mode = !display_mode;
      ILI9341_clear(BLACK);
    }
  }
  if (GPIO_Pin == KEY_ENCODER_Pin)
  {
    static uint32_t last_enc_time = 0;
    if (current_time - last_enc_time > 200)
    {
      last_enc_time = current_time;
      if (current_focus == FOCUS_TRIGGER) current_focus = FOCUS_VERTICAL;
      else if (current_focus == FOCUS_VERTICAL) current_focus = FOCUS_TIMEBASE;
      else if (current_focus == FOCUS_TIMEBASE) current_focus = FOCUS_COUPLING;
      else if (current_focus == FOCUS_COUPLING) current_focus = FOCUS_CALIBRATION;
      else if (current_focus == FOCUS_CALIBRATION) current_focus = FOCUS_TRIGGER;
    }
  }
  if (GPIO_Pin == KEY_AUTO_Pin)
  {
    static uint32_t last_mode_time = 0;
    if (current_time - last_mode_time > 200)
    {
      auto_flag = 1;
    }
  }
  if (GPIO_Pin == KEY_CH_TOGGLE_Pin)
  {
    static uint32_t last_toggle_time = 0;
    if (current_time - last_toggle_time > 200)
    {
      last_toggle_time = current_time;
      g_ui_display_channel = (g_ui_display_channel == 1) ? 2 : 1;
    }
  }
}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
