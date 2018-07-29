#ifndef _EXTERNALINTERRUPTION_H_
#define _EXTERNALINTERRUPTION_H_

#include "stm32f0xx.h"
#include "App.h"

extern App_Event_T EXTI4_Event;

void EXIT_PA0_Config(void);
void EXIT0_1_NVIC_Config(uint8_t Priority);
void EXTI0_1_IRQ(void);

void EXIT_PA4_Config(void);
void EXIT4_15_NVIC_Config(uint8_t Priority);
void EXTI4_15_IRQ(void);

#endif
