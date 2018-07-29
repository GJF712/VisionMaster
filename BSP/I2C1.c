#include "I2C1.h"

#include <stddef.h>
#include <string.h>
#include <math.h>

/* 变量 ----------------------------------------------------------------------*/
#define I2C_TIMEOUT 249
#define I2C_Sa_TIMER	0xafff

//#define Buffer_Len 16
//static uint8_t  Buffer_Rx[Buffer_Len];
//static uint8_t  Buffer_Tx[Buffer_Len];
//static float ADC_CH0_DataBuff;
//const static uint8_t I2C1_DataLen[I2C1_Num_Max_ID] = {6, 3, 6, 6};	//表示要写目标数据的长度
//static uint8_t I2C1_Func_ID;
//static uint16_t I2C1_Show_ID;
//#ifdef DEBUG_DISABLE
//	static uint8_t key_data = 0;
//#endif
static void I2C1_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	/* Connect PA9 to I2C_SCL */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);
	/* Configure PA10 pins: SDA */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	

    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void I2C1_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure;

	I2C_StructInit(&I2C_InitStructure);
	
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Timing = 0xb0420f13;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x7f<<1;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);

	I2C_Cmd(I2C1, ENABLE);//使能I2C
}

void I2C1_Init(void)
{
	I2C1_GPIOInit();
	I2C1_Config();
}

//==========================================================================================================================

//==========================================================================================================================
static void I2C_CheckBusy(void){
	uint16_t I2C_Timeout = 0;
	
	static uint8_t Count = 0;
	if((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET) && (Count++ > 2)){
		I2C_Cmd(I2C1, DISABLE);
		while(I2C_Timeout++ < I2C_TIMEOUT);
		I2C_Cmd(I2C1, ENABLE);
		Count = 0;
	}else if((Count != 0) && (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == RESET)){
		Count = 0;
	}
//	if(I2C_GetFlagStatus(I2C1, I2C_FLAG_ARLO) == SET){
//		I2C_ClearFlag(I2C1, I2C_FLAG_ARLO);
//	}
}

uint16_t IIC_WriteData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len){
	uint16_t timer = 0;
	uint16_t Index = 0;
	I2C_TransferHandling(I2C1, SlaveAddress << 1, len + 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

	timer = I2C_Sa_TIMER;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET){
		if((timer--) == 0) {
			goto b;
			}
	}
	I2C_SendData(I2C1, DataAddress);
	
	while(Index < len){
		timer = I2C_Sa_TIMER;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET){
			if((timer--) == 0) {
				goto b;
			}
		}
		I2C_SendData(I2C1, buf[Index++]);
	}

	timer = I2C_Sa_TIMER;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
		if((timer--) == 0){
			Index = 0; 
			goto b;
		}

	I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
	
b:	I2C_CheckBusy();
	return Index;
}

uint16_t IIC_ReadData(uint8_t SlaveAddress, uint8_t DataAddress, uint8_t *buf, uint16_t len){
	uint16_t timer = 0;
	uint16_t Index = 0;

	I2C_TransferHandling(I2C1, SlaveAddress << 1, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
	
	timer = I2C_Sa_TIMER;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET){
		if((timer--) == 0) {
			goto b;
		}
	}
	I2C_SendData(I2C1, DataAddress);

	timer = I2C_Sa_TIMER;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET){
		if((timer--) == 0) {
			goto b;
		}
	}
	I2C_TransferHandling(I2C1, 0xf<<1, len, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	while(len--){
		timer = I2C_Sa_TIMER;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
			if((timer--) == 0) {
				goto b;
			}
		buf[Index++] = I2C_ReceiveData(I2C1);
	}
	
	timer = I2C_Sa_TIMER;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
		if((timer--) == 0) {
			Index = 0;
			goto b;
		}
		
	I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);

b:	I2C_CheckBusy();
	return Index;
}

//void I2C1_GetData_EventTask(uint8_t DataAddress, uint16_t Len){//处理读回来的数据
//	if((((DataAddress >> 4) - 1) < I2C1_Num_Max_ID) && (Len == I2C1_DataLen[(DataAddress >> 4) - 1])&& (Sum_Checking(Buffer_Rx, Len - 1) == Buffer_Rx[Len - 1])){
//		switch(DataAddress){
//			case Calibration_ID://校准
//				break;
//        	case Relay_ID:
//				Set_RelayState(Buffer_Rx[I2C1_Data_First_ID]);
//        		break;
//        	case Input_4_20MA_ID:
//				memcpy(&ADC_CH0_DataBuff, &Buffer_Rx[I2C1_Data_First_ID], sizeof(float));
//        		break;
//        	case Output_4_20MA_ID:
//				Set_DAC4_20MA_Data(*(float *)&Buffer_Rx[I2C1_Data_First_ID]);
//        		break;
//        	default:
//        		break;
//        }
//	}
//}
