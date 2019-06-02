#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_OK 3
#define KEY_CANCEL 4
#define KEY_LEFT 5
#define KEY_RIGHT 6
void KEY_GPIO_Config(void);
/********************检测接上拉电阻的按键**************************/
u8 Key_Scan_L(GPIO_TypeDef*GPIOx,u16 GPIO_pin);
/********************检测接下拉电阻的按键**************************/
u8 Key_Scan_H(GPIO_TypeDef*GPIOx,u16 GPIO_pin);
#endif
