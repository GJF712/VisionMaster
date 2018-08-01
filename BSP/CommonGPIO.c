/*
 * led.c
 *
 *  Created on: 2018��6��25��
 *      Author: Junfeng.Guo
 */

#include "CommonGPIO.h"
#include "multi_button.h" 
#include "App.h"
#include "I2C1.h"
#include "ADC1.h"

//==============================================================================================
//���Ե�
//==============================================================================================
void LED_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void LED_Flicker(void){
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4)));
}

//==============================================================================================
//ָʾ��
//==============================================================================================
void LEDs_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_7);
	
	Ctrl_LEDFunc(LED_G, LED_ON_STATE);
}

void Ctrl_LEDFunc(Ctrl_LED_COLOR_T Color, Ctrl_LED_STATE_T State){
	if(Color != LED_R){
		GPIO_WriteBit(GPIOA, LED_R, (BitAction)LED_OFF_STATE);
	}
	if(Color != LED_G){
		GPIO_WriteBit(GPIOA, LED_G, (BitAction)LED_OFF_STATE);
	}
	if(Color != LED_B){
		GPIO_WriteBit(GPIOA, LED_B, (BitAction)LED_OFF_STATE);
	}
	GPIO_WriteBit(GPIOA, Color, (BitAction)State);
}

void Ctrl_LEDsFunc(void){
	typedef struct{
		Schedule_TimerDataType PWMMS_Timer;
		Ctrl_LED_STATE_T State;
	}LED_PWM_Flicker_T;
	typedef struct{
		Schedule_TimerDataType MS_Timer;
		LED_PWM_Flicker_T PWM_F[2];
	}LED_Flicker_T;
	const static LED_Flicker_T LED_FlickerCtrl[] = {
		{3000, {{0, LED_ON_STATE}, {10, LED_OFF_STATE}}},
		{100, {{1, LED_ON_STATE}, {9, LED_OFF_STATE}}},
		{100, {{2, LED_ON_STATE}, {8, LED_OFF_STATE}}},
		{100, {{3, LED_ON_STATE}, {7, LED_OFF_STATE}}},
		{100, {{4, LED_ON_STATE}, {6, LED_OFF_STATE}}},
		{100, {{5, LED_ON_STATE}, {5, LED_OFF_STATE}}},
		{100, {{6, LED_ON_STATE}, {4, LED_OFF_STATE}}},
		{100, {{7, LED_ON_STATE}, {3, LED_OFF_STATE}}},
		{100, {{8, LED_ON_STATE}, {2, LED_OFF_STATE}}},
		{100, {{9, LED_ON_STATE}, {1, LED_OFF_STATE}}},
		{100, {{10, LED_ON_STATE}, {0, LED_OFF_STATE}}},
		{100, {{10, LED_ON_STATE}, {0, LED_OFF_STATE}}},
		{100, {{9, LED_ON_STATE}, {1, LED_OFF_STATE}}},
		{100, {{8, LED_ON_STATE}, {2, LED_OFF_STATE}}},
		{100, {{7, LED_ON_STATE}, {3, LED_OFF_STATE}}},
		{100, {{6, LED_ON_STATE}, {4, LED_OFF_STATE}}},
		{100, {{5, LED_ON_STATE}, {5, LED_OFF_STATE}}},
		{100, {{4, LED_ON_STATE}, {6, LED_OFF_STATE}}},
		{100, {{3, LED_ON_STATE}, {7, LED_OFF_STATE}}},
		{100, {{2, LED_ON_STATE}, {8, LED_OFF_STATE}}},
		{100, {{1, LED_ON_STATE}, {9, LED_OFF_STATE}}},
		{0,},//����
	};
	static Schedule_TimerDataType Last_TimerCounter = 0;
	static Schedule_TimerDataType Last_PWMTimerCounter = 0;
	
	if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == SET) && (VCC_BAT4S_V != BatteryVoltage)){
		Ctrl_LEDFunc(LED_B, LED_ON_STATE);
		Last_TimerCounter = Get_Schedule_MS();
	}else if(VCC_BAT4S_V < BatteryVoltage * 0.2){
		Ctrl_LEDFunc(LED_R, LED_ON_STATE);
		Last_PWMTimerCounter = Get_Schedule_MS();
	}else{
		static uint8_t SwitchLight = 0;
		static LED_Flicker_T const *pLED_FlickerCtrl = &LED_FlickerCtrl[0];
		static LED_PWM_Flicker_T const *pLED_PWM_FlickerCtrl = LED_FlickerCtrl[0].PWM_F;
		
		if(MS_TimerTrigger(&Last_TimerCounter, pLED_FlickerCtrl->MS_Timer)){
			pLED_FlickerCtrl++;
			if(pLED_FlickerCtrl->MS_Timer == 0){
				pLED_FlickerCtrl = &LED_FlickerCtrl[0];
			}
		}
		if(MS_TimerTrigger(&Last_PWMTimerCounter, pLED_PWM_FlickerCtrl->PWMMS_Timer)){
			SwitchLight = !SwitchLight;
			pLED_PWM_FlickerCtrl = &pLED_FlickerCtrl->PWM_F[SwitchLight];
			if(pLED_PWM_FlickerCtrl->PWMMS_Timer != 0){
				Ctrl_LEDFunc(LED_G, pLED_PWM_FlickerCtrl->State);
			}
		}
	}
}

//==============================================================================================
//TX2��������
//==============================================================================================
void TX2_POWER_BTNInit(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TX2_POWER_ON(void){
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void TX2_POWER_OFF(void){
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

//==============================================================================================
//���ð���
//==============================================================================================
static struct button Button_Key;
static uint8_t Key_PA0_State(void){
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

static void Callback_Long_Click_Handler(void* btn){
//	/* ʹ��WKUP���ŵĻ��ѹ��� */
//	PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);
//	/* �������ģʽ */
//	PWR_EnterSTANDBYMode();
}

static void Callback_Press_Down_Handler(void* btn){
	TX2_POWER_ON();
}

static void Callback_Press_Up_Handler(void* btn){
	TX2_POWER_OFF();
}

void Key_PA0_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	button_init(&Button_Key, Key_PA0_State, 1);
	button_attach(&Button_Key, LONG_PRESS_HOLD, Callback_Long_Click_Handler);
	button_attach(&Button_Key, PRESS_DOWN, Callback_Press_Down_Handler);
	button_attach(&Button_Key, PRESS_UP, Callback_Press_Up_Handler);
	button_start(&Button_Key);
}

