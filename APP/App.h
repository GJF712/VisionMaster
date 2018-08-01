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

typedef struct{
	bool Event_flag;
	void (* const APP_Function)(bool *Event_flag);
}App_Event_T;

extern volatile Schedule_TimerDataType Schedule_MS;
extern App_Event_T TX2_ON_Event;

void App_Init(void);
bool MS_TimerTrigger(Schedule_TimerDataType * const Last_TimerCounter, Schedule_TimerDataType const MS_Timer);
Schedule_TimerDataType Get_Schedule_MS(void);
void TX2_ON(bool *Event_flag);
void Set_TX2_State(bool State);//1 ON,0 OFF

#endif
