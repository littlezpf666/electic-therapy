#include "key.h"
#include "delay.h"
void KEY_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启GPIOD的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	
	/*选择要控制的GPIOD引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;	

	/*设置引脚模式为通用悬浮输入*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*设置引脚速率为50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	
	/*调用库函数，初始化GPIOD*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
/***************************************************************************/
  	/*选择要控制的GPIOC引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	

	/*设置引脚模式为通用悬浮输入*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*设置引脚速率为50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOC*/
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	
/***************************************************************************/
	
  	/*选择要控制的GPIOA引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_0;	

	/*设置引脚模式为通用悬浮输入*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   

	/*设置引脚速率为50MHz */   
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOA*/
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