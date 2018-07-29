#include "App.h"

volatile Schedule_TimerDataType Schedule_MS = 0;

bool MS_TimerTrigger(Schedule_TimerDataType * const Last_TimerCounter, Schedule_TimerDataType const MS_Timer){
	bool Result = false;
	if(Schedule_MS < *Last_TimerCounter){
		Result = (((Schedule_TimerDataType)(-1) - *Last_TimerCounter + Schedule_MS) >= MS_Timer);
	}else{
		Result = ((Schedule_MS - *Last_TimerCounter) >= MS_Timer);
	}
	if(Result){
		*Last_TimerCounter = Schedule_MS;
	}
	return Result;
}

/*****************************************************************************
应用层初始化
******************************************************************************
*****************************************************************************/
void App_Init(void)
{
}
