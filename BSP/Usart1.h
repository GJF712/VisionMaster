#ifndef _USART1_H_
#define _USART1_H_

#include "stm32f0xx.h"

typedef struct{
	uint8_t RXFinshFlag;
	uint8_t Len;
	uint8_t Data[128];
}USART1_DATA_T;

void USART1_Init(void);
void USART1_NVIC_Config(uint8_t Priority);
void USART1_IRQ(void);
void USART1_SendBuf(const uint8_t *pBuf, uint8_t u8Len);
USART1_DATA_T *Set_Usart1Data(void);
void USART1_RXDisable(void);
void USART1_RXEnable(void);

#endif
