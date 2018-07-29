#include "I2C1.h"

#include <stddef.h>
#include <string.h>
#include <math.h>

/* ���� ----------------------------------------------------------------------*/
#define I2C_TIMEOUT 249
#define I2C_Sa_TIMER	0xafff

//#define Buffer_Len 16
//static uint8_t  Buffer_Rx[Buffer_Len];
//static uint8_t  Buffer_Tx[Buffer_Len];
//static float ADC_CH0_DataBuff;
//const static uint8_t I2C1_DataLen[I2C1_Num_Max_ID] = {6, 3, 6, 6};	//��ʾҪдĿ�����ݵĳ���
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

	I2C_Cmd(I2C1, ENABLE);//ʹ��I2C
}

void I2C1_Init(void)
{
	I2C1_GPIOInit();
	I2C1_Config();
}

//==========================================================================================================================

//==========================================================================================================================
void I2C_CheckBusy(void){
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


//=====================================================================================
//Ӧ�ò㴦��
//=====================================================================================
#define TUSB422_I2CAddr	0x40
uint8_t I2C_Buff[16];
void TUSB422_BoostTo20V(void){
	uint16_t VoltageScale = 20 / 5;
	I2C_Buff[0] = VoltageScale & 0xff;
	I2C_Buff[1] = (VoltageScale >> 8) & 0xff;
	if(IIC_WriteData(TUSB422_I2CAddr, 0x70, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(0x40, 0x70, I2C_Buff, 2) != 2){
		}
	}
}

#define BQ25703A_I2CAddr 0x12

static void MinSystemVoltage(void){
	uint16_t Voltage = 12800;
	I2C_Buff[0] = Voltage & 0x00;
	I2C_Buff[1] = (Voltage >> 8) & 0x3f;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x0C, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x0C, I2C_Buff, 2) != 2){
		}
	}
}

static void ChargeCurrent(void){//�������ó�1000mA��ʵ����960mA
	uint16_t Current = 1000;
	I2C_Buff[0] = Current & 0xC0;
	I2C_Buff[1] = (Current >> 8) & 0x3F;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x02, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x02, I2C_Buff, 2) != 2){
		}
	}
}

static void MaxChargeVoltage(void){
	uint16_t Voltage = 16800;
	I2C_Buff[0] = Voltage & 0xf0;
	I2C_Buff[1] = (Voltage >> 8) & 0x7f;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x04, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x04, I2C_Buff, 2) != 2){
		}
	}
}

static void OTGCurrent(void){
	uint16_t Current = 2000;
	I2C_Buff[0] = 0;
	I2C_Buff[1] = Current / 50;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x08, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x08, I2C_Buff, 2) != 2){
		}
	}
}

static void OTGVoltage(void){//�������ó�20000mV��ʵ����19968mA
	uint16_t Voltage = 20000 - 4480;
	I2C_Buff[0] = Voltage & 0xC0;
	I2C_Buff[1] = (Voltage >> 8) & 0x3f;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x06, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x06, I2C_Buff, 2) != 2){
		}
	}
}
	
static void IIN_DPM(void){
	uint16_t Current = 3250;
	I2C_Buff[0] = 0;
	I2C_Buff[1] = (Current / 50) & 0x7f;
	if(IIC_WriteData(BQ25703A_I2CAddr, 0x24, I2C_Buff, 2) != 2){		
		if(IIC_WriteData(BQ25703A_I2CAddr, 0x24, I2C_Buff, 2) != 2){
		}
	}
}

void BQ25703A_Init(void){
	IIN_DPM();
	OTGVoltage();
	OTGCurrent();
	MaxChargeVoltage();
	ChargeCurrent();
	MinSystemVoltage();
}
