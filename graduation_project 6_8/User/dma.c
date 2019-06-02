#include"dma.h"
#include"nvic.h"

uint8_t SendBuff[SENDBUFF_SIZE];

volatile uint16_t ADC_ConvertedValue;

void DMA_Config(void)
{ 
	/****************************USART DMA����***********************************/
	DMA_InitTypeDef DMA_InitStructure;
	/*��DMA1ʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	/*�����жϿ�����*/
	NVIC_Configuration();
	/*�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr=USART2_DR_Base;
	/*�ڴ��ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;
	/*���䷽�򣺴��ڴ浽����*/
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
	/*�������ݸ���*/
	DMA_InitStructure.DMA_BufferSize=SENDBUFF_SIZE;
	/*�����ַ����*/
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;
	/*�������ݵ�λ*/
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	/*�ڴ����ݵ�λ*/
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	/*DMAģʽ��һ�δ����ѭ��*/
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	/*���ȼ�����*/
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;
	/*��ֹ�ڴ浽�ڴ�֮��Ĵ���*/
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	/*��ʼ��DMA*/
	DMA_Init(DMA1_Channel7,&DMA_InitStructure);
	/*ʹ��DMA*/
//	DMA_Cmd(DMA1_Channel7,ENABLE);
/***********DMA��������ж�ʹ��*****************/
//	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
	
	
	/****************************ADC DMA����***********************************/
	DMA_DeInit(DMA1_Channel1);
		/*�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;
	/*�ڴ��ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;
	/*���䷽��:�����赽�ڴ�*/
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	/*�������ݸ���*/
	DMA_InitStructure.DMA_BufferSize=1;
	/*�����ַ����*/
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;
	/*�������ݵ�λ*/
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	/*�ڴ����ݵ�λ*/
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	/*DMAģʽ��һ�δ����ѭ��*/
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	/*���ȼ�����*/
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	/*��ֹ�ڴ浽�ڴ�֮��Ĵ���*/
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	/*��ʼ��DMA*/
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	/*ʹ��DMA*/
	DMA_Cmd(DMA1_Channel1,ENABLE);
	/***********DMA��������ж�ʹ��*****************/
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
}
