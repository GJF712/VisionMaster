/*
 * led.h
 *
 *  Created on: 2018Äê6ÔÂ25ÈÕ
 *      Author: Junfeng.Guo
 */

#ifndef _COMMONGPIO_H_
#define _COMMONGPIO_H_

#include "stm32f0xx.h"

typedef enum{
	LED_G = GPIO_Pin_5,
	LED_R = GPIO_Pin_6,
	LED_B = GPIO_Pin_7,
	LED_Max,
}Ctrl_LED_COLOR_T;

typedef enum{
	LED_ON_STATE,
	LED_OFF_STATE,
	LED_MAX_STATE,
}Ctrl_LED_STATE_T;

void LED_Init(void);
void LED_Flicker(void);

void Key_PA0_INIT(void);

#endif /* BSP_LED_H_ */
