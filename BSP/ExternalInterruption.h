#ifndef _EXTERNALINTERRUPTION_H_
#define _EXTERNALINTERRUPTION_H_

#include "stm32f0xx.h"

void EXIT_PA0_Config(void);
void EXIT0_NVIC_Config(uint8_t Priority);
void EXTI0_1_IRQ(void);

#endif
