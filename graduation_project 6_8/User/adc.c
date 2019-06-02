#include "adc.h"

void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
static void ADC1_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开GPIOB，ADC1时钟*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB,ENABLE);
	/*设置引脚PB.0模式为模拟输入*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*调用库函数，初始化GPIOB*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	/*独立模式*/
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	/*扫描模式*/
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	/*连续转换*/
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	/*是否使用外部触发转换*/
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	/*采集数据右对齐*/
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	/*要转换的通道数目*/
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_InitStructure);
	/*配置ADC时钟*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//8分频
	/*配置ADC的通道采样周期*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);//通道8 55.5个采样周期
	/*使能ADC1 DMA*/
	ADC_DMACmd(ADC1,ENABLE);
	/*使能ADC1*/
	ADC_Cmd(ADC1,ENABLE);
	/*复位校准寄存器*/
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*ADC校准*/
	ADC_StartCalibration(ADC1);
	/*等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	/*软件触发*/
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/**
  * @brief  This function handles ADC signal preiod value .
  * @param  threshold 用于检测周期的阈值 AD_value 采集的AD值
  * @retval counter_result 采集的AD值的个数
  */

#define wave_state_0 0
#define wave_state_1 1
uint16_t preiod(uint16_t threshold,uint16_t AD_value)/*到返回值的函数没有return返回随机数*/
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
