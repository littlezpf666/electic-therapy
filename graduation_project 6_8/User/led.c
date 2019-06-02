#include "led.h"
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOD������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIOD����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOC*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);		  

	/* �ر�����led��	*/
	GPIO_ResetBits(GPIOD, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);	 
}

uint8_t Count_KEY=0;
void LED_TOGGLE(void)
{
	  if(++Count_KEY==1)
		GPIO_WriteBit(GPIOD, GPIO_Pin_8,Bit_SET);
		else
		{ 
      GPIO_WriteBit(GPIOD, GPIO_Pin_8,Bit_RESET);
			Count_KEY=0;
		}
}	
