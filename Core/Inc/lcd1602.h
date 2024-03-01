/*
 * lcd1602.h
 *
 *  Created on: Feb 28, 2024
 *      Author: benja
 */

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

#include <string.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

/* delay timer rate macro */
#define DELAY_TIMER_RATE		42

/* lcd pin macros */
#define RS_PORT 				GPIOB
#define RS_PIN 					GPIO_PIN_7
//#define RW_PORT
//#define RW_PIN
#define E_PORT 					GPIOA
#define E_PIN 					GPIO_PIN_15
#define DB4_PORT 				GPIOA
#define DB4_PIN 				GPIO_PIN_4
#define DB5_PORT 				GPIOB
#define DB5_PIN 				GPIO_PIN_0
#define DB6_PORT 				GPIOC
#define DB6_PIN 				GPIO_PIN_1
#define DB7_PORT 				GPIOC
#define DB7_PIN 				GPIO_PIN_0

/* rs pin macros */
#define RS_CMD					0
#define RS_DATA					1

/* cmd bit setting macros */
#define CMD_CONFIG				0x28 // interface length 4 bits, 2 lines, 5x8 font
#define CMD_CLEAR				0x01
#define CMD_DISPLAY_ON			0x0C // display on, cursor not visible, cursor blink off
#define CMD_DISPLAY_OFF			0x08
#define CMD_CURSOR_RIGHT		0x06 // moves cursor right after each character

/* command delay times */
#define CMD_CLEAR_DELAY			2000
#define CMD_CURSOR_DELAY		40

/* lcd functions */

// delays processor for specified number of us
void lcdDelayUs(uint16_t us);

// sends command
void lcdSendCommand(uint8_t cmd);

// moves cursor to specified row and column
void lcdMoveCursor(uint8_t row, uint8_t column);

//clears screen
void lcdClear(void);

void lcdSendString(char *str);

void lcdInit(void);

#endif /* INC_LCD1602_H_ */
