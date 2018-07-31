/*
 * led.h
 *
 *  Created on: 2018年6月25日
 *      Author: Junfeng.Guo
 */

#ifndef _COMMONGPIO_H_
#define _COMMONGPIO_H_

#include "stm32f0xx.h"

typedef enum{
	LED_T = GPIO_Pin_4,
	LED_G = GPIO_Pin_5,
	LED_R = GPIO_Pin_6,
	LED_B = GPIO_Pin_7,
	LED_Max,
}Ctrl_LED_COLOR_T;

//共阳发光二极管
typedef enum{
	LED_ON_STATE,
	LED_OFF_STATE,
	LED_MAX_STATE,
}Ctrl_LED_STATE_T;

void LED_Init(void);
void LED_Flicker(void);

void LEDs_Init(void);
void Ctrl_LEDFunc(Ctrl_LED_COLOR_T Color, Ctrl_LED_STATE_T State);
void Ctrl_LEDsFunc(void);

void TX2_POWER_BTNInit(void);
void TX2_POWER_ON(void);
void TX2_POWER_OFF(void);

void Key_PA0_Init(void);

#endif /* BSP_LED_H_ */
