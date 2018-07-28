#include "Usart1.h"
#include "Timer.h"

USART1_DATA_T Usart1_Data;

USART1_DATA_T *Set_Usart1Data(void){
	return &Usart1_Data;
}


/*****************************************************************************

******************************************************************************
*****************************************************************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//����PA10�ɵڶ���������  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_StructInit(&USART_InitStructure);
	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = 19200;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
    
//    USART_SetReceiverTimeOut(USART1, (8+1) * 5);        //���ý��ճ�ʱ���������5���ַ�����ʱ��û�����ݱ����գ�����Ϊ��ʱ��
//    USART_ReceiverTimeOutCmd(USART1, DISABLE);           //���ճ�ʱ����
//    USART_ITConfig(USART1, USART_IT_RTO, DISABLE);       //���ճ�ʱ�ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //�����ж�
//    USART_ITConfig(USART1, USART_IT_TC, DISABLE);       //��������ж�
//    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);      //�����ж�
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
	USART_ITConfig(USART1, USART_IT_FE, ENABLE);
	USART_ITConfig(USART1, USART_IT_CM, ENABLE);
	USART_ITConfig(USART1, USART_IT_NE, ENABLE);
	USART_Cmd(USART1, ENABLE);                          //ʹ��USART1
}

void USART1_RXEnable(void){
	USART_ReceiveData(USART1);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //�����ж�
//	USART_Cmd(USART1, ENABLE); 
}

void USART1_RXDisable(void){
//	USART_Cmd(USART1, DISABLE); 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);     //�����ж�
}

void USART1_NVIC_Config(uint8_t Priority)
{
	NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = Priority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void USART1_SendBuf(const uint8_t *pBuf, uint8_t u8Len)
{
    while(u8Len)
    {
        /*�жϷ��ͻ������Ƿ�Ϊ��*/
        while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
        USART_SendData(USART1,*pBuf++);
        u8Len--;
    }
}

void USART1_SendByte(const uint8_t Buf)
{
	/*�жϷ��ͻ������Ƿ�Ϊ��*/
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
	USART_SendData(USART1,Buf);
}

void USART1_IRQ(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
    	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        if(Usart1_Data.Len == 128){
        	Usart1_Data.Len = 0;
        }
        Usart1_Data.Data[Usart1_Data.Len] = USART_ReceiveData(USART1);
//		USART1_SendByte(Usart1_Data.Data[Usart1_Data.Len]);
		Usart1_Data.Len++;
        TIM14_Compare1Set(10);
    }
    if(USART_GetITStatus(USART1, USART_IT_ORE) == SET)
    {
        USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_ORE);
    }
    if(USART_GetITStatus(USART1, USART_IT_FE) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_FE);
    }
    if(USART_GetITStatus(USART1, USART_IT_CM) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_CM);
    }
    if(USART_GetITStatus(USART1, USART_IT_NE) == SET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_NE);
    }
}
