#include "key.h"
#include "delay.h"
void KEY_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����GPIOD������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	
	/*ѡ��Ҫ���Ƶ�GPIOD����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	

	/*��������ģʽΪͨ����������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*������������Ϊ50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	/*���ÿ⺯������ʼ��GPIOD*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
/***************************************************************************/
  	/*ѡ��Ҫ���Ƶ�GPIOC����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	

	/*��������ģʽΪͨ����������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*������������Ϊ50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOC*/
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	
/***************************************************************************/
	
  	/*ѡ��Ҫ���Ƶ�GPIOA����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_0;	

	/*��������ģʽΪͨ����������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*������������Ϊ50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	
}
u8 Key_Scan_L(GPIO_TypeDef*GPIOx,u16 GPIO_pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==0)
		{
			Delay(10000);
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==0)
			{
				while(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==0);
				     return 0;
			}
			else   return 1;
		}
		else   return 1;
}
u8 Key_Scan_H(GPIO_TypeDef*GPIOx,u16 GPIO_pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==1)
		{
			Delay(10000);
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==1)
			{
				while(GPIO_ReadInputDataBit(GPIOx,GPIO_pin)==1);
				     return 0;
			}
			else   return 1;
		}
		else   return 1;
}