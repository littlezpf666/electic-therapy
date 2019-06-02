#include "usart2.h"

void USART2_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/*ÉèÖÃTx¿Ú*/
	/*Ñ¡ÔñÒª¿ØÖÆµÄGPIOAÒý½Å*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	/*ÉèÖÃÒý½ÅÄ£Ê½Îª¸´ÓÃÍÆÍìÊä³ö*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
	/*ÉèÖÃÒý½ÅËÙÂÊÎª50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*µ÷ÓÃ¿âº¯Êý£¬³õÊ¼»¯GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*ÉèÖÃRx¿Ú*/
	/*Ñ¡ÔñÒª¿ØÖÆµÄGPIOAÒý½Å*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	/*ÉèÖÃÒý½ÅÄ£Ê½ÎªÐü¸¡ÊäÈë*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	/*µ÷ÓÃ¿âº¯Êý£¬³õÊ¼»¯GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*³õÊ¼»¯USART*/	
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	/*USART DMAÊ¹ÄÜ*/
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	USART_NVIC_Configuration();
	/*USARTÊ¹ÄÜ*/
	USART_Cmd(USART2,ENABLE);
	
}

int fputc(int ch,FILE*f)
{
	USART_SendData(USART2, (unsigned char)ch);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	return (ch);
}

/*********************·¢ËÍÒ»´®Êý¾Ý******************************/
void Usart2_Put_Buf(u8 a[],u8 _cnt) 
{
  u8 i;
  for(i=0;i<_cnt;i++) 
	{
	USART_SendData(USART2,a[i]);
	while (USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);	
  }
}

/*********************»æÍ¼ÉÏÎ»»ú·¢ËÍ´¦Àí*************************/
void Data_Send_Senser(u16 a,u16 b,u16 c,u16 d)
{
	u8  _cnt=0,i,sum=0;
  u8 		data_to_send[30];
	data_to_send[_cnt++]=0xAA;//??í·
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=a/256;  //16?? êy?Y·?3é??°?µí°? ?????ú?°
	data_to_send[_cnt++]=a%256;  //µí???úoó
	
	data_to_send[_cnt++]=b/256;  //í¨µà2
	data_to_send[_cnt++]=b%256;
	
	data_to_send[_cnt++]=c/256;  // í¨µà3
	data_to_send[_cnt++]=c%256;
	
	data_to_send[_cnt++]=d/256;
	data_to_send[_cnt++]=d%256;
		
	data_to_send[3] = _cnt-4;
	
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	Usart2_Put_Buf(data_to_send,_cnt);
}