#include"dma.h"
#include"nvic.h"

uint8_t SendBuff[SENDBUFF_SIZE];

volatile uint16_t ADC_ConvertedValue;

void DMA_Config(void)
{ 
	/****************************USART DMA配置***********************************/
	DMA_InitTypeDef DMA_InitStructure;
	/*开DMA1时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	/*配置中断控制器*/
	NVIC_Configuration();
	/*外设地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr=USART2_DR_Base;
	/*内存地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;
	/*传输方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
	/*发送数据个数*/
	DMA_InitStructure.DMA_BufferSize=SENDBUFF_SIZE;
	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	/*内存地址不增*/
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;
	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	/*内存数据单位*/
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	/*DMA模式：一次传输或循环*/
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	/*优先级：中*/
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;
	/*禁止内存到内存之间的传输*/
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	/*初始化DMA*/
	DMA_Init(DMA1_Channel7,&DMA_InitStructure);
	/*使能DMA*/
//	DMA_Cmd(DMA1_Channel7,ENABLE);
/***********DMA发送完成中断使能*****************/
//	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
	
	
	/****************************ADC DMA配置***********************************/
	DMA_DeInit(DMA1_Channel1);
		/*外设地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;
	/*内存地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;
	/*传输方向:从外设到内存*/
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	/*发送数据个数*/
	DMA_InitStructure.DMA_BufferSize=1;
	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	/*内存地址不增*/
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;
	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	/*内存数据单位*/
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	/*DMA模式：一次传输或循环*/
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	/*优先级：高*/
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	/*禁止内存到内存之间的传输*/
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	/*初始化DMA*/
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	/*使能DMA*/
	DMA_Cmd(DMA1_Channel1,ENABLE);
	/***********DMA发送完成中断使能*****************/
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
}
