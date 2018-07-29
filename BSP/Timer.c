/*
 * Timer.c
 *
 *  Created on: 2018年6月25日
 *      Author: Junfeng.Guo
 */

#include "Timer.h"
#include <limits.h>
#include "Usart1.h"
#include "App.h"

void TIM14_Compare1Set(uint16_t _0_1ms){//单位0.1ms
    TIM_SetCompare1(TIM14,TIM_GetCounter(TIM14) + _0_1ms);
    TIM_ITConfig(TIM14,TIM_IT_CC1,ENABLE);
    TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);
}

void TIM14_Compare1Stop(void){
    TIM_ITConfig(TIM14,TIM_IT_CC1,DISABLE);
    TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);
}

//===============================================================

void TIM3_Config(void)//1ms
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = 2 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 24000 - 1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void TIM14_Config(void){//0.1ms
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = USHRT_MAX;
    TIM_TimeBaseInitStruct.TIM_Prescaler = SystemCoreClock / 10000;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);

    TIM_Cmd(TIM14, ENABLE);
}

void TIM16_PWM(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = 480 - 1;
	TIM_TimeBaseStructure.TIM_Period =  2000 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_Pulse = 150;//以0.01ms为单位
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM16, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM16, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
}

void TIM17_PWM(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_OCStructInit(&TIM_OCInitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = 480 - 1;
	TIM_TimeBaseStructure.TIM_Period =  2000 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_Pulse = 150;//以0.01ms为单位
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM17, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM17, ENABLE);
	TIM_Cmd(TIM17, ENABLE);
	TIM_CtrlPWMOutputs(TIM17, ENABLE);
}

void TIM3_NVIC_Config(uint8_t Priority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = Priority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void TIM14_NVIC_Config(uint8_t Priority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = Priority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void TIM3_IRQ(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		Schedule_MS++;
    }
}

void TIM14_IRQ(void)
{
    if(TIM_GetITStatus(TIM14, TIM_IT_CC1) == SET)
    {
        TIM_ClearFlag(TIM14,TIM_FLAG_CC1);
        TIM14_Compare1Stop();
        USART1_RXDisable();
        Set_Usart1Data()->RXFinshFlag = 1;
    }
}
