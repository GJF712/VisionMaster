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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_StructInit(&USART_InitStructure);
	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = 19200;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
    
//    USART_SetReceiverTimeOut(USART1, (8+1) * 5);        //设置接收超时（如果超过5个字符接收时间没有数据被接收，则判为超时）
//    USART_ReceiverTimeOutCmd(USART1, DISABLE);           //接收超时报警
//    USART_ITConfig(USART1, USART_IT_RTO, DISABLE);       //接收超时中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //接收中断
//    USART_ITConfig(USART1, USART_IT_TC, DISABLE);       //发送完成中断
//    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);      //发送中断
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
	USART_ITConfig(USART1, USART_IT_FE, ENABLE);
	USART_ITConfig(USART1, USART_IT_CM, ENABLE);
	USART_ITConfig(USART1, USART_IT_NE, ENABLE);
	USART_Cmd(USART1, ENABLE);                          //使能USART1
}

void USART1_RXEnable(void){
	USART_ReceiveData(USART1);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     //接收中断
//	USART_Cmd(USART1, ENABLE); 
}

void USART1_RXDisable(void){
//	USART_Cmd(USART1, DISABLE); 
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);     //接收中断
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
        /*判断发送缓冲区是否为空*/
        while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
        USART_SendData(USART1,*pBuf++);
        u8Len--;
    }
}

void USART1_SendByte(const uint8_t Buf)
{
	/*判断发送缓冲区是否为空*/
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
