#include "adc.h"

void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
static void ADC1_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��GPIOB��ADC1ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB,ENABLE);
	/*��������PB.0ģʽΪģ������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*���ÿ⺯������ʼ��GPIOB*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	/*����ģʽ*/
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	/*ɨ��ģʽ*/
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	/*����ת��*/
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	/*�Ƿ�ʹ���ⲿ����ת��*/
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	/*�ɼ������Ҷ���*/
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	/*Ҫת����ͨ����Ŀ*/
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_InitStructure);
	/*����ADCʱ��*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//8��Ƶ
	/*����ADC��ͨ����������*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);//ͨ��8 55.5����������
	/*ʹ��ADC1 DMA*/
	ADC_DMACmd(ADC1,ENABLE);
	/*ʹ��ADC1*/
	ADC_Cmd(ADC1,ENABLE);
	/*��λУ׼�Ĵ���*/
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*ADCУ׼*/
	ADC_StartCalibration(ADC1);
	/*�ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	/*�������*/
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/**
  * @brief  This function handles ADC signal preiod value .
  * @param  threshold ���ڼ�����ڵ���ֵ AD_value �ɼ���ADֵ
  * @retval counter_result �ɼ���ADֵ�ĸ���
  */

#define wave_state_0 0
#define wave_state_1 1
uint16_t preiod(uint16_t threshold,uint16_t AD_value)/*������ֵ�ĺ���û��return���������*/
{
	static uint8_t wave_state=0;
	static uint16_t AD_NUM_counter=0;
	static uint16_t counter_result=0;
	switch (wave_state)
	{
		case wave_state_0:
			AD_NUM_counter++;
			if(AD_value>threshold)
			{ 
				counter_result=AD_NUM_counter;
				AD_NUM_counter=0;
				wave_state=1;
			}				
			break;
		case wave_state_1:
			AD_NUM_counter++;
			if(AD_value<threshold)
				wave_state=0;
		  break;
	}	
	return counter_result;
}
