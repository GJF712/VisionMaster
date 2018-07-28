/*
 * bsp.c
 *
 *  Created on: 2018Äê6ÔÂ26ÈÕ
 *      Author: Junfeng.Guo
 */

#include "bsp.h"
#include "Timer.h"
#include "led.h"
#include "Usart1.h"
#include "ExternalInterruption.h"


void Bsp_Init(void){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	
	LED_Init();

    TIM16_PWM();
    TIM17_PWM();

	TIM3_Config();
	TIM3_NVIC_Config(0);

	TIM14_Config();
	TIM14_NVIC_Config(1);

	USART1_Init();
	USART1_NVIC_Config(2);
	
	EXIT_PA0_Config();
	EXIT0_NVIC_Config(3);
}
