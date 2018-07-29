/*
 * bsp.c
 *
 *  Created on: 2018��6��26��
 *      Author: Junfeng.Guo
 */

#include "bsp.h"
#include "Timer.h"
#include "CommonGPIO.h"
#include "Usart1.h"
#include "ExternalInterruption.h"

void Bsp_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET){//�������Ѹ�λ
		PWR_ClearFlag(PWR_FLAG_WU);
	}else{//�ϵ縴λ
		/* ʹ��WKUP���ŵĻ��ѹ��� */
		PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
		/* �������ģʽ */
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
