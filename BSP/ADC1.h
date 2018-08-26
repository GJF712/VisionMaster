#ifndef _ADC1_H_
#define _ADC1_H_

#include "stm32f0xx.h"

extern float VCC_BAT4S_V,  VCC_BOARD_V;

void ADC1_StartTask(void);
void ADC1_Init(void);
void ADC1_NVIC_Config(uint8_t Priority);
void ADC1_IRQ(void);

#endif
