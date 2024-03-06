/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define DWT_CTRL			( *(volatile uint32_t*) (0xE0001000) )
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
TaskHandle_t printTaskHandle, startTimerTaskHandle, rtcUpdateTaskHandle, rtcSetTaskHandle, alarmSetTaskHandle;
QueueHandle_t printQueueHandle;
TimerHandle_t printTimerHandle;

displayMode_t currMode = mDisplayRtc;
selected_t currSet = sHour;

RTC_DateTypeDef rtcDate;
RTC_TimeTypeDef rtcTime;

RTC_DateTypeDef setDate;
RTC_TimeTypeDef setTime;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void printTimerCallback(TimerHandle_t xTimer);

void startTimerTaskHandler(void *parameters);
void printTaskHandler(void *parameters);
void rtcUpdateTaskHandler(void *parameters);
void rtcSetTaskHandler(void *parameters);
void alarmSetTaskHandler(void *parameters);

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
  BaseType_t status; /* holds task creation status */
  /* USER CODE END 1 */

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
  MX_RTC_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  //DWT_CTRL |= (1 << 0); //enable CYCCNT counter (cycle count counter)

  HAL_TIM_Base_Start(&htim1);

  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();

  /* create queues */
  printQueueHandle = xQueueCreate(PRINT_QUEUE_LEN, sizeof(size_t) ); /* size of size_t (32 bits) because print queue holds pointer to char (string) */
  configASSERT(printQueueHandle != NULL);

  /*create timers */
  printTimerHandle = xTimerCreate("Print_Timer", pdMS_TO_TICKS(RTC_SAMPLE_PERIOD), pdTRUE, NULL, printTimerCallback);

  /* create tasks */
  status = xTaskCreate(startTimerTaskHandler, "Start_Timer_Task", 250, NULL, 2, &startTimerTaskHandle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(printTaskHandler, "Print_Task", 250, NULL, 3, &printTaskHandle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(rtcUpdateTaskHandler, "RTC_Update_Task", 250, NULL, 2, &rtcUpdateTaskHandle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(rtcSetTaskHandler, "RTC_Set_Task", 250, NULL, 2, &rtcSetTaskHandle);
  configASSERT(status == pdPASS);

  status = xTaskCreate(alarmSetTaskHandler, "Alarm_Set_Task", 250, NULL, 2, &alarmSetTaskHandle);
  configASSERT(status == pdPASS);

  lcdInit();

  vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DB7_Pin|DB6_Pin|BUZZER_Pin|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DB4_Pin|E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DB5_Pin|RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DB7_Pin DB6_Pin BUZZER_Pin PC10 */
  GPIO_InitStruct.Pin = DB7_Pin|DB6_Pin|BUZZER_Pin|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DB4_Pin E_Pin */
  GPIO_InitStruct.Pin = DB4_Pin|E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : INC_Pin DEC_Pin */
  GPIO_InitStruct.Pin = INC_Pin|DEC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DB5_Pin RS_Pin */
  GPIO_InitStruct.Pin = DB5_Pin|RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MODE_Pin SELECT_Pin */
  GPIO_InitStruct.Pin = MODE_Pin|SELECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

	void printTaskHandler(void *parameters){
		uint32_t *str;

		while(1){
			/* print top row */
			xQueueReceive(printQueueHandle, &str, portMAX_DELAY);
			lcdClear();
			lcdMoveCursor(0, 0);
			lcdSendString( (char*) str );

			/* print bottom row */
			xQueueReceive(printQueueHandle, &str, portMAX_DELAY);
			lcdMoveCursor(1, 0);
			lcdSendString( (char*) str );
		}
	}

	void startTimerTaskHandler(void *parameters){
		while(1){
			xTimerStart(printTimerHandle, portMAX_DELAY);
			vTaskSuspend(startTimerTaskHandle);
		}
	}

	void printTimerCallback(TimerHandle_t xTimer){
		if (currMode == mDisplayRtc){
			xTaskNotify(rtcUpdateTaskHandle, 0, eNoAction);
		}
		else if (currMode == mSetRtc){
			xTaskNotify(rtcSetTaskHandle, 0, eNoAction);
		}
		else if(currMode == mSetAlarm){
			xTaskNotify(alarmSetTaskHandle, 0, eNoAction);
		}
	}

	void vApplicationIdleHook(void){
		if(HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin) == GPIO_PIN_SET){
			if (currMode == mDisplayRtc){
				currMode = mSetRtc;
				HAL_Delay(DEBOUNCE_DELAY_PERIOD);
			}
			else if (currMode == mSetRtc){
				currMode = mSetAlarm;
				HAL_Delay(DEBOUNCE_DELAY_PERIOD);
			}
			else if(currMode == mSetAlarm){
				currMode = mDisplayRtc;
				HAL_Delay(DEBOUNCE_DELAY_PERIOD);
			}
		}

//		if (HAL_GPIO_ReadPin(INC_GPIO_Port, INC_Pin) == GPIO_PIN_SET && currMode == mSetRtc) {
//
//		}
	}

	void rtcUpdateTaskHandler(void *parameters){
		static char strBuffer[40];
		static char *str = strBuffer;

		while(1){
			xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

			memset( &rtcDate,0,sizeof(rtcDate) );
			memset( &rtcTime,0,sizeof(rtcTime) );

			HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
			HAL_RTC_GetTime(&hrtc, &setTime, RTC_FORMAT_BIN); // sets time for "set" mode

			HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &setDate, RTC_FORMAT_BIN); // sets time for "set" mode

			char *format;
			format = (rtcTime.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

			memset(&strBuffer, 0, sizeof(strBuffer) );

			sprintf( (char*) strBuffer, "%02d:%02d:%02d [%s]",rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds, format);
			SEGGER_SYSVIEW_PrintfTarget(str);
			xQueueSend(printQueueHandle, &str, portMAX_DELAY);

			memset(&strBuffer, 0, sizeof(strBuffer) );
			sprintf( (char*) strBuffer, "%02d-%02d-%2d", rtcDate.Month, rtcDate.Date, 2000 + rtcDate.Year);
			SEGGER_SYSVIEW_PrintfTarget(str);
			xQueueSend(printQueueHandle, &str, portMAX_DELAY);

		}
	}

	void rtcSetTaskHandler(void *parameters){
		static char strBuffer[40];
		static char *str = strBuffer;

		while(1){
			xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

			char *format;
			format = (setTime.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

			memset(&strBuffer, 0, sizeof(strBuffer) );

			sprintf( (char*) strBuffer, "%02d:%02d:%02d [%s]",setTime.Hours, setTime.Minutes, setTime.Seconds, format);
			xQueueSend(printQueueHandle, &str, portMAX_DELAY);

			memset(&strBuffer, 0, sizeof(strBuffer) );
			sprintf( (char*) strBuffer, "%02d-%02d-%2d", setDate.Month, setDate.Date, 2000 + setDate.Year);
			xQueueSend(printQueueHandle, &str, portMAX_DELAY);

		}
	}

	void alarmSetTaskHandler(void *parameters){
		static char strBuffer[40];
		static char *str = strBuffer;

		while(1){
			xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

			memset(&strBuffer, 0, sizeof(strBuffer) );
			sprintf( (char*) strBuffer, "Alarm" );
			xQueueSend(printQueueHandle, &str , portMAX_DELAY);

			memset(&strBuffer, 0, sizeof(strBuffer) );
			sprintf( (char*) strBuffer, "Mode" );
			xQueueSend(printQueueHandle, &str , portMAX_DELAY);

		}
	}


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM5 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM5) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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

#ifdef  USE_FULL_ASSERT
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
