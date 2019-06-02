#include "stm32f10x.h"
#include "led.h"
//#include "exti.h"
#include "key.h"
#include "usart2.h"
#include "dma.h"
#include "adc.h"
#include "dac.h"
#include "delay.h"
#include "ili_lcd_general.h"
#include "lcd_api.h"
#include "math.h"
#include "screen_sel.h"
#include "tim.h"
#include "syn6288.h"

extern uint8_t  SendBuff[SENDBUFF_SIZE];
extern uint8_t mode_step,stop_stage;
extern volatile uint16_t ADC_ConvertedValue;
extern volatile uint16_t Preiod_value;
extern uint16_t DDS_step;
extern uint16_t si[][256],sawtooth[][256],triangle[][256],rectangel[][256],ex[][256];
extern char scr_state,wave_pattern,amplitude_level;
uint16_t flag1=0,flag2=0,flag3=0;
int main(void)
{ 
	uint16_t Heart_rate;
	uint16_t DAC_value;
	uint16_t module_stage=0;
	uint16_t time;
	USART2_Config();
	DAC1_Init();
	//LCD初始化
  lcd_Initializtion();
/***********************************开机界面*************************************************/
	LCD_write_ASCII16x24_string(0,4+24*2,"Information",Blue,White);
	LCD_write_ASCII16x24_string(32,4+24*4,"14-1 36",Blue,White);
	LCD_write_ASCII16x24_string(48,4+24*6,"Pengfei Zhao",Red,White);
	
	 /****************************各模块配置初始化****************************************/
	LED_GPIO_Config();
	KEY_GPIO_Config();
	DMA_Config();
	ADC1_Init();
	
	TIM7_Mode_config();
	Delay(3000000);
	lcd_clear(White);
	screen_display(0,0,0);
	syn6288_SpeakStr("开机",0);
	/*******************产生波形DA值************************/
//	for(time=0;time<256;time++)
//	{
//	 si[1][time]=2000*sin(6.28/255*time)+2048;
//	}
//  for(time=0;time<256;time++)
//	{
//		printf("%d,",si[1][time]);
//	}
// 0.7mV/1单位
//	for(time=0;time<256;time++)
//	{
//	 sawtooth[0][time]=1400/256*time+2048-700;
//	}
//  for(time=0;time<256;time++)
//	{
//		printf("%d,",sawtooth[0][time]);
//	}
//   A/128x=y -A/128x+2A=y
//		for(time=0;time<256;time++)
//	{
//	 if(time<128)
//	 triangle[0][time]=4200/128*time+2048-2100;
//	 else
//	 triangle[0][time]=-4200/128*time+8400+2048-2100;
//	}
//  for(time=0;time<256;time++)
//	{
//		printf("%d,",triangle[0][time]);
//	}
//   e(x)
//		for(time=0;time<256;time++)
//	{
//	 if(time<128)
//	 ex[0][time]=exp((double)7.6/128*time)+2048;
//	 else
//	 ex[0][time]=-exp((double)7.6/128*(time-128))+2048;
//	}
//  for(time=0;time<256;time++)
//	{
//		printf("%d,",ex[0][time]);
//	}
	while(1)
	{   
		  /*确定*/
		  if(Key_Scan_L(GPIOD,GPIO_Pin_0)==0)
		  { 
				screen_display(scr_state,DETECT_KEY,KEY_OK);
				GPIO_WriteBit(GPIOD, GPIO_Pin_8, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_8)));
			}	
			/*向上*/
			if(Key_Scan_L(GPIOD,GPIO_Pin_3)==0)
		  { 
				GPIO_WriteBit(GPIOD, GPIO_Pin_9, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_9)));
				screen_display(scr_state,DETECT_KEY,KEY_UP);
			}
			
			/*向下*/
			if(Key_Scan_L(GPIOD,GPIO_Pin_2)==0)
		  { 
				GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_11)));
				screen_display(scr_state,DETECT_KEY,KEY_DOWN);
			}
			/*退出*/
			if(Key_Scan_L(GPIOC,GPIO_Pin_13)==0)
		  { 
				GPIO_WriteBit(GPIOD, GPIO_Pin_11, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_11)));
				screen_display(scr_state,DETECT_KEY,KEY_CANCEL);
			}
				/*向左*/
			if(Key_Scan_L(GPIOD,GPIO_Pin_4)==0)
		  { 
				screen_display(scr_state,DETECT_KEY,KEY_LEFT);
				
			}	
			 /*向右*/
			if(Key_Scan_L(GPIOD,GPIO_Pin_1)==0)
		  { 
				screen_display(scr_state,DETECT_KEY,KEY_RIGHT);
				
			}

			if((amplitude_level!=0)&&(stop_stage!=1))
			{
				switch(wave_pattern)
				{	
				case PATTERN_MASSAGE:
				  if(mode_step==0)
					{
						DAC_SetChannel1Data(DAC_Align_12b_R,triangle[amplitude_level-1][DDS_step]);
					}
					else if(mode_step==1)
					{
						if(DDS_step<128)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048+amplitude_level*312);
						if(DDS_step>127)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048-amplitude_level*312);
					}
					break;
				case PATTERN_STROKE:
					if(DDS_step<128)
					DAC_SetChannel1Data(DAC_Align_12b_R,2048+amplitude_level*312);
					if(DDS_step>127)
					DAC_SetChannel1Data(DAC_Align_12b_R,2048-amplitude_level*312);
					break;
				case PATTERN_CUPPING:
					if(mode_step==0)
					{
						if(DDS_step<128)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048+amplitude_level*312);
						if(DDS_step>127)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048-amplitude_level*312);
					}
					else if(mode_step==1)
					{
						DAC_SetChannel1Data(DAC_Align_12b_R,ex[amplitude_level-1][DDS_step]);
					}
				  
					break;
				case PATTERN_ACUPUNCTURE:
					if(mode_step==0)
					{
					  DAC_SetChannel1Data(DAC_Align_12b_R,si[amplitude_level-1][DDS_step]);
					}
					else if(mode_step==1)
					{
						DAC_SetChannel1Data(DAC_Align_12b_R,sawtooth[amplitude_level-1][DDS_step]);
					}	
					
					break;
				case PATTERN_SCRAPPING:
				  if(mode_step==0)
					{
						if(DDS_step<61)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048+amplitude_level*312);
						if(DDS_step>60)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048-amplitude_level*312);
					}
					else if(mode_step==1)
					{
						if(DDS_step<128)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048+amplitude_level*312);
						if(DDS_step>127)
						DAC_SetChannel1Data(DAC_Align_12b_R,2048-amplitude_level*312);
					}
					break;
				}
			}
      else
			{
			  DAC_SetChannel1Data(DAC_Align_12b_R,2048);
			}	
      if(scr_state==screen_2)
      {				
		  /*(239.5+12.5)/9*150*Preiod_value=一个周期多少us
			72MHZ 8分频239.5个采样周期软件150分频*/
			Heart_rate=600000/(Preiod_value*42);//心脏一分钟跳多少次
			if(Heart_rate>30&&Heart_rate<170)
			value_display(110,4,Heart_rate);
			}
//			if(Heart_rate>30&&Heart_rate<170)
//			{
//			printf("\r\n %d \r\n",Heart_rate);
//			}
//      printf("\r\n The Value is : %d \r\n",DAC_GetDataOutputValue(DAC_Channel_1));
	}
}


