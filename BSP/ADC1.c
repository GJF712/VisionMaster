#include "ADC1.h"

#define SAMPLE_COUNT 10
static uint16_t VCC_BAT4S_D[SAMPLE_COUNT], VCC_BOARD_D[SAMPLE_COUNT];
float VCC_BAT4S_V,  VCC_BOARD_V;

void ADC1_CH1_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC1_CH9_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC1_Init(void){
    ADC_InitTypeDef ADC_InitStructure;
	
	ADC_StructInit(&ADC_InitStructure);
	ADC_DeInit(ADC1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);//使用专用的ADC时钟
	
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;    
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_239_5Cycles);

    ADC_GetCalibrationFactor(ADC1);
    ADC_Cmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_OverrunModeCmd(ADC1, DISABLE);
    
    ADC_StartOfConversion(ADC1);
}

void ADC1_NVIC_Config(uint8_t Priority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = Priority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void ADC1_StartTask(void){
	uint32_t sum;
	uint8_t Counting = 0;
	uint16_t DelayCount = 0;
	while((ADC1->CR & ADC_CR_ADSTART) && (DelayCount++ < 0x8fff)){}//防止死锁
//	ADC1->CHSELR = ((ADC1->CHSELR == ADC_Channel_1) ? ADC_Channel_9 : ADC_Channel_1);
	if(ADC1->CHSELR == ADC_Channel_1){//VCC_BOARD
		ADC1->CHSELR = ADC_Channel_9;
		
		while(Counting < SAMPLE_COUNT){
			sum += VCC_BOARD_D[Counting++];
		}
		VCC_BOARD_V = 3.3 * sum / SAMPLE_COUNT / 4096 * (1);//括号内是电阻分压系数
	}else{//VCC_BAT4S
		ADC1->CHSELR = ADC_Channel_1;
		
		while(Counting < SAMPLE_COUNT){
			sum += VCC_BAT4S_D[Counting++];
		}
		VCC_BAT4S_V = 3.3 * sum / SAMPLE_COUNT / 4096 * (1);//括号内是电阻分压系数
	}
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) == SET){
		ADC_ClearFlag(ADC1, ADC_FLAG_OVR);
	}
	ADC_StartOfConversion(ADC1);
}

void ADC1_IRQ(void){
	static uint8_t VCC_BOARD_C = 0, VCC_BAT4S_C = 0;
	uint16_t ADC1_Value;
    if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET){
        ADC1_Value = ADC_GetConversionValue(ADC1);
		ADC_StopOfConversion(ADC1);
		switch(ADC1->CHSELR){
			case ADC_Channel_1://VCC_BOARD
				VCC_BOARD_D[VCC_BOARD_C] = ADC1_Value;
				if(++VCC_BOARD_C == SAMPLE_COUNT){
					VCC_BOARD_C = 0;
				}
				
				break;
			case ADC_Channel_9://VCC_BAT4S
				VCC_BAT4S_D[VCC_BAT4S_C] = ADC1_Value;
				if(++VCC_BAT4S_C == SAMPLE_COUNT){
					VCC_BAT4S_C = 0;
				}
				break;
			default:
				break;
		}
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}
