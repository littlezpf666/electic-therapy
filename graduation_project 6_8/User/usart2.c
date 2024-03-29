#include "usart2.h"

void USART2_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/*设置Tx口*/
	/*选择要控制的GPIOA引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	/*设置引脚模式为复用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*设置Rx口*/
	/*选择要控制的GPIOA引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	/*设置引脚模式为悬浮输入*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	/*调用库函数，初始化GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*初始化USART*/	
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/*USART DMA使能*/
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	USART_NVIC_Configuration();
	/*USART使能*/
	USART_Cmd(USART2,ENABLE);
	
}

int fputc(int ch,FILE*f)
{
	USART_SendData(USART2, (unsigned char)ch);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	return (ch);
}

/*********************发送一串数据******************************/
void Usart2_Put_Buf(u8 a[],u8 _cnt) 
{
  u8 i;
  for(i=0;i<_cnt;i++) 
	{
	USART_SendData(USART2,a[i]);
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);	
  }
}

/*********************绘图上位机发送处理*************************/
void Data_Send_Senser(u16 a,u16 b,u16 c,u16 d)
{
	u8  _cnt=0,i,sum=0;
  u8 		data_to_send[30];
	data_to_send[_cnt++]=0xAA;//??矸
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=a/256;  //16?? 陏?Y�?3�??�?淀�? ?????�?�
	data_to_send[_cnt++]=a%256;  //淀???鷒�
	
	data_to_send[_cnt++]=b/256;  //悫掂2
	data_to_send[_cnt++]=b%256;
	
	data_to_send[_cnt++]=c/256;  // 悫掂3
	data_to_send[_cnt++]=c%256;
	
	data_to_send[_cnt++]=d/256;
	data_to_send[_cnt++]=d%256;
		
	data_to_send[3] = _cnt-4;
	
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	Usart2_Put_Buf(data_to_send,_cnt);
}