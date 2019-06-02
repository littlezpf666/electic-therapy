#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include "stdio.h"
#include "nvic.h"

void USART2_Config(void);
int fputc(int ch,FILE*f);
void Data_Send_Senser(u16 a,u16 b,u16 c,u16 d);
#endif