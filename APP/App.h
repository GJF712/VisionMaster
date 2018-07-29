#ifndef _APP_H_
#define _APP_H_

#include "stm32f0xx.h"
#include <stdbool.h>

#define Schedule_TimerDataType uint16_t

typedef struct{
	Schedule_TimerDataType MS_Counting;
	const Schedule_TimerDataType MS_Timer;
	void (* const APP_Function)(void);
}App_Schedule_T;

extern volatile Schedule_TimerDataType Schedule_MS;

void App_Init(void);
bool MS_TimerTrigger(Schedule_TimerDataType * const Last_TimerCounter, Schedule_TimerDataType const * const MS_Timer);

#endif
