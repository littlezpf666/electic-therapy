
#include "tim.h"

void TIM6_Mode_config()
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStucture;
//	TIM_OCInitTypeDef TIM_OCInitStucture;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	 //390us���һ��256��0.1s
	TIM_TimeBaseStucture.TIM_Period=390; 
	TIM_TimeBaseStucture.TIM_Prescaler=71;
//	TIM_TimeBaseStucture.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseStucture.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStucture);
	TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
	/*********************�ж�����***************************/
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
  TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_NVIC_Configuration();
}
void TIM7_Mode_config()
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStucture;
//	TIM_OCInitTypeDef TIM_OCInitStucture;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	TIM_TimeBaseStucture.TIM_Period=1000;//������ֵ 
	TIM_TimeBaseStucture.TIM_Prescaler=71;//72MHz��71+1����ƵΪ1MHz
//	TIM_TimeBaseStucture.TIM_ClockDivision=TIM_CKD_DIV1;//��6��7��ʱ��������ʱ������
//	TIM_TimeBaseStucture.TIM_CounterMode=TIM_CounterMode_Up;//������ʱ��ֻ�����ϼ���һ��ģʽ
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStucture);
/*********************�ж�����***************************/
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
  TIM_ITConfig(TIM7,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM7, DISABLE);
	TIM_NVIC_Configuration();
}

