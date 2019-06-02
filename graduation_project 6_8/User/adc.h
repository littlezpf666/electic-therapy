#ifndef __ADC_H
#define __ADC_H
 
#include"stm32f10x.h"

void ADC1_Init(void);
static void ADC1_GPIO_Config();
static void ADC1_Mode_Config();
uint16_t preiod(uint16_t threshold,uint16_t AD_value);
#endif
