/*
 * bsp.c
 *
 *  Created on: 2018年6月26日
 *      Author: Junfeng.Guo
 */

#include "bsp.h"
#include "Timer.h"
#include "CommonGPIO.h"
#include "Usart1.h"
#include "ExternalInterruption.h"

void Bsp_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET){//待机唤醒复位
		PWR_ClearFlag(PWR_FLAG_WU);
	}else{//上电复位
		/* 使能WKUP引脚的唤醒功能 */
		PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
		/* 进入待机模式 */
		PWR_EnterSTANDBYMode();
	}
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF, ENABLE);
	
	LED_Init();

    TIM16_PWM();
    TIM17_PWM();

	TIM3_Config();
	TIM3_NVIC_Config(0);

	TIM14_Config();
	TIM14_NVIC_Config(1);

	USART1_Init();
	USART1_NVIC_Config(2);
	
//	EXIT_PA0_Config();
//	EXIT0_NVIC_Config(3);
	
	Key_PA0_INIT();
}
