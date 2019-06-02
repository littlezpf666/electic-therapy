#ifndef __DMA_H
#define __DMA_H
 
#include"stm32f10x.h"

#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404
#define ADC1_DR_Address ((u32)0x40012400+0x4c)
#define SENDBUFF_SIZE 2000

void DMA_Config(void);

#endif
