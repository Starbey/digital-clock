/*
 * task_handlers.c
 *
 *  Created on: Mar 6, 2024
 *      Author: benja
 */

#include "main.h"

static void handleSetTime(RTC_TimeTypeDef *setTime);
static void handleSetDate(RTC_DateTypeDef *setDate);
static void handleSelect(uint8_t numOptions);

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
		HAL_RTC_GetAlarm(&hrtc, &rtcAlarm, RTC_ALARM_A, RTC_FORMAT_BIN);
		vTaskSuspend(alarmBuzzerTaskHandle);
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
		currSet = sHour;

		currMode++;
		currMode = currMode % 3;

		HAL_Delay(DEBOUNCE_DELAY_PERIOD);
	}

	else if (currMode == mSetRtc) {
		if(currSet <= 2){
			handleSetTime(&setTime);
		}
		else {
			handleSetDate(&setDate);
		}
		handleSelect(6);
	}

	else if (currMode == mSetAlarm){
		handleSetTime(& (rtcAlarm.AlarmTime) );
		handleSelect(3);
	}
}

static void handleSetTime(RTC_TimeTypeDef *setTime){
	if (HAL_GPIO_ReadPin(INC_GPIO_Port, INC_Pin) == GPIO_PIN_SET){
		if(currSet == sHour){
			if(setTime->Hours < 23) setTime->Hours++;
		}
		else if(currSet == sMin){
			if(setTime->Minutes < 59) setTime->Minutes++;
		}
		else if(currSet ==sSec){
			if(setTime->Seconds < 59) setTime->Seconds++;
		}
		HAL_Delay(DEBOUNCE_DELAY_PERIOD); // putting delay here improves responsiveness for some reason
	}

	else if (HAL_GPIO_ReadPin(DEC_GPIO_Port, DEC_Pin) == GPIO_PIN_SET){
		if(currSet == sHour){
			if(setTime->Hours > 0) setTime->Hours--;
		}
		else if(currSet == sMin){
			if(setTime->Minutes > 0) setTime->Minutes--;
		}
		else if(currSet == sSec){
			if(setTime->Seconds > 0) setTime->Seconds--;
		}
		HAL_Delay(DEBOUNCE_DELAY_PERIOD); // putting delay here improves responsiveness for some reason
	}
}

static void handleSetDate(RTC_DateTypeDef *setDate){
	if (HAL_GPIO_ReadPin(INC_GPIO_Port, INC_Pin) == GPIO_PIN_SET){
		if(currSet == sMonth){
			if(setDate->Month < 11) setDate->Month++;
		}
		else if(currSet == sDay) {
			if(setDate->Date < 30) setDate->Date++;
		}
		else if(currSet == sYear) {
			if(setDate->Year < 998) setDate->Year++;
		}
		HAL_Delay(DEBOUNCE_DELAY_PERIOD); // putting delay here improves responsiveness for some reason
	}

	else if (HAL_GPIO_ReadPin(DEC_GPIO_Port, DEC_Pin) == GPIO_PIN_SET){
		if(currSet == sMonth){
			if(setDate->Month > 1) setDate->Month--;
		}
		else if(currSet == sDay) {
			if(setDate->Date > 1) setDate->Date--;
		}
		else if(currSet == sYear) {
			if(setDate->Year > 0) setDate->Year--;
		}
		HAL_Delay(DEBOUNCE_DELAY_PERIOD); // putting delay here improves responsiveness for some reason
	}
}

static void handleSelect(uint8_t numOptions){
	if(HAL_GPIO_ReadPin(SELECT_GPIO_Port, SELECT_Pin) == GPIO_PIN_SET){
		currSet++;
		currSet = currSet % numOptions;

		HAL_Delay(DEBOUNCE_DELAY_PERIOD);
	}
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
		HAL_RTC_GetDate(&hrtc, &setDate, RTC_FORMAT_BIN); // sets date for "set" mode

		memset(&strBuffer, 0, sizeof(strBuffer) );

		sprintf( (char*) strBuffer, "%02d:%02d:%02d",rtcTime.Hours, rtcTime.Minutes, rtcTime.Seconds);
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

		HAL_RTC_SetTime(&hrtc, &setTime, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &setDate, RTC_FORMAT_BIN);

		memset(&strBuffer, 0, sizeof(strBuffer) );

		sprintf( (char*) strBuffer, "%02d:%02d:%02d",setTime.Hours, setTime.Minutes, setTime.Seconds);
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

		HAL_RTC_SetAlarm_IT(&hrtc, &rtcAlarm, RTC_FORMAT_BIN); // update alarm

		memset(&strBuffer, 0, sizeof(strBuffer) );

		sprintf( (char*) strBuffer, "Alarm ON" );
		xQueueSend(printQueueHandle, &str , portMAX_DELAY);

		memset(&strBuffer, 0, sizeof(strBuffer) );
		sprintf( (char*) strBuffer, "%02d:%02d:%02d", rtcAlarm.AlarmTime.Hours, rtcAlarm.AlarmTime.Minutes, rtcAlarm.AlarmTime.Seconds);
		xQueueSend(printQueueHandle, &str , portMAX_DELAY);

	}
}

/* starts buzzer */
void alarmStartTaskHandler(void *parameters){
	while(1){
		vTaskSuspend(NULL);

		xTimerStart(alarmTimerHandle, portMAX_DELAY);
		xTimerStart(alarmLedTimerHandle, portMAX_DELAY);
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET); // ensure LED is on as soon as alarm starts
		vTaskResume(alarmBuzzerTaskHandle);
	}
}

/* determines length of alarm, not auto-reload */
void alarmTimerCallback(TimerHandle_t xTimer){
	xTimerStop(alarmLedTimerHandle, portMAX_DELAY);
	vTaskSuspend(alarmBuzzerTaskHandle);

	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
}

/* determines LED frequency */
void alarmLedTimerCallback(TimerHandle_t xTimer){
	HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
}

/* determines buzzer frequency */
void alarmBuzzerTaskHandler(void *parameters){
	while(1){
		HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
		delayUs(ALARM_BUZZ_PERIOD);
	}
}
