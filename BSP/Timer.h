/*
 * Timer.h
 *
 *  Created on: 2018Äê6ÔÂ25ÈÕ
 *      Author: Junfeng.Guo
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f0xx.h"

void TIM3_Config(void);
void TIM3_NVIC_Config(uint8_t Priority);
void TIM3_IRQ(void);

void TIM14_Compare1Stop(void);
void TIM14_Compare1Set(uint16_t _0_1ms);
void TIM14_Config(void);
void TIM14_NVIC_Config(uint8_t Priority);
void TIM14_IRQ(void);

void TIM16_PWM(void);
void TIM17_PWM(void);

#endif /* TIMER_H_ */
