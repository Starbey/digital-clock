/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd1602.h"
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "time.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum{
	mDisplayRtc = 0,
	mSetRtc,
	mSetAlarm
}displayMode_t;

typedef enum{
	sHour = 0,
	sMin,
	sSec,
	sMonth,
	sDay,
	sYear
}selected_t;

extern RTC_HandleTypeDef hrtc;

extern TaskHandle_t printTaskHandle, startTimerTaskHandle, rtcUpdateTaskHandle, rtcSetTaskHandle, alarmSetTaskHandle, alarmBuzzerTaskHandle;
extern QueueHandle_t printQueueHandle;
extern TimerHandle_t printTimerHandle, alarmTimerHandle, alarmLedTimerHandle;

extern displayMode_t currMode;
extern selected_t currSet;
extern RTC_DateTypeDef rtcDate;
extern RTC_TimeTypeDef rtcTime;

extern RTC_DateTypeDef setDate;
extern RTC_TimeTypeDef setTime;

extern RTC_AlarmTypeDef rtcAlarm;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define PRINT_QUEUE_LEN				10
#define RTC_SAMPLE_PERIOD			500

#define DEBOUNCE_DELAY_PERIOD		300

#define ALARM_SAMPLE_PERIOD			500
#define ALARM_LEN					5000
#define ALARM_LED_PERIOD			500
#define ALARM_BUZZ_PERIOD			300
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delayUs(uint16_t us);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define DB7_Pin GPIO_PIN_0
#define DB7_GPIO_Port GPIOC
#define DB6_Pin GPIO_PIN_1
#define DB6_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DB4_Pin GPIO_PIN_4
#define DB4_GPIO_Port GPIOA
#define INC_Pin GPIO_PIN_5
#define INC_GPIO_Port GPIOA
#define DEC_Pin GPIO_PIN_6
#define DEC_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_7
#define BUZZER_GPIO_Port GPIOA
#define DB5_Pin GPIO_PIN_0
#define DB5_GPIO_Port GPIOB
#define RED_LED_Pin GPIO_PIN_9
#define RED_LED_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_15
#define E_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define RS_Pin GPIO_PIN_7
#define RS_GPIO_Port GPIOB
#define MODE_Pin GPIO_PIN_8
#define MODE_GPIO_Port GPIOB
#define SELECT_Pin GPIO_PIN_9
#define SELECT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
