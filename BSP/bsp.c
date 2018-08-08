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
#include "I2C1.h"

void Bsp_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	if(PWR_GetFlagStatus(PWR_FLAG_WU) == SET){//待机唤醒复位
		PWR_ClearFlag(PWR_FLAG_WU);
		Set_TX2_State(true);
	}else{//上电复位
//		/* 使能WKUP引脚的唤醒功能 */
//		PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
//		/* 进入待机模式 */
//		PWR_EnterSTANDBYMode();
		Set_TX2_State(true);
	}
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF, ENABLE);
	
	LEDs_Init();
	
	TX2_POWER_BTNInit();

	TIM3_Config();
	TIM3_NVIC_Config(0);

	I2C1_Init();

//	TIM14_Config();
//	TIM14_NVIC_Config(1);

//	USART1_Init();
//	USART1_NVIC_Config(2);
	
//	EXIT_PA4_Config();
//	EXIT4_15_NVIC_Config(3);
	
	Key_PA0_Init();
	
	BQ25703A_Init();
}
