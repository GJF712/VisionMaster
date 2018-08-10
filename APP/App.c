#include "App.h"
#include "CommonGPIO.h"
#include "ExternalInterruption.h"
#include "I2C1.h"

volatile Schedule_TimerDataType Schedule_MS = 0;
App_Event_T TX2_ON_Event = {false, TX2_ON};
App_Event_T TX2_OFF_Event = {false, TX2_OFF};
uint8_t TX2_ONOFF_State = 0;

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

Schedule_TimerDataType Get_Schedule_MS(void){
	return Schedule_MS;
}
//=========================================================================
static Schedule_TimerDataType Last_Set_TX2;

void Set_TX2_State(bool State){//1 ON,0 OFF
	Last_Set_TX2 = Get_Schedule_MS();
	if(State){
		TX2_ON_Event.Event_flag = true;
		TX2_ONOFF_State = 1;
	}else{
		TX2_OFF_Event.Event_flag = true;
		TX2_ONOFF_State = 0;
	}
}

void TX2_ON(bool *Event_flag){
	TX2_POWER_ON();
	if(MS_TimerTrigger(&Last_Set_TX2, 500)){
		*Event_flag = false;
		TX2_POWER_OFF();
	}
}

void TX2_OFF(bool *Event_flag){
	TX2_POWER_ON();
	if(MS_TimerTrigger(&Last_Set_TX2, 10000)){
		*Event_flag = false;
		TX2_POWER_OFF();
	}
}

/*****************************************************************************
应用层初始化
******************************************************************************
*****************************************************************************/
void App_Init(void){
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == SET)
	{
		TUSB422_BoostTo20VInit();
	}
}
