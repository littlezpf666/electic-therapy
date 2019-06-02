/**********************屏幕显示函数****************************/
//*********************************************************
#include "screen_sel.h"
//*********************************************************
unsigned int sel_state=0,scr_state=0;
char wave_pattern=0,amplitude_level=0,time_interval=5;
//---------------------------------------------------------
  char pchar0[]="MANUAL";
	char pchar1[]="AUTOMATIC";
	char pchar2[]="MASSAGE";
	char pchar3[]="STROKE";
	char pchar4[]="CUPPING";
	char pchar5[]="ACUPUNCTURE";
	char pchar6[]="SCRAPPING";
	char pchar7[]="AMPLITUDE";
	char pchar8[]="FREQUENCE";
	char pchar9[]="TIME";
	char pchar10[]="Heart Rate:";
	char *pstr[]={pchar0,pchar1,pchar2,pchar3,pchar4,pchar5,pchar6,pchar7,pchar8,pchar9};
//---------------------------------------------------------
//引用变量要与源变量声明的类型相同
extern unsigned int tim7_counter1,tim7_counter2;
extern uint16_t DDS_step,DDSM;	
/*********************************************************
 *函数名称：change_sel
 *函数功能：上下移动，（选中光标）
 *输入参数：step
 *输出参数：无
 *********************************************************/

//---------------------------------------------------------
void change_sel(unsigned int scr,int step)
{
	if(step >0)
	{ 
		  if(scr==screen_0)
			{
				if(sel_state==1)sel_state=0;
					else sel_state++;
			}
			if(scr==screen_1)
			{
				if(sel_state==4)sel_state=0;
					else sel_state++;
			}
		  if(scr==screen_2)
			{
				if(sel_state==2)sel_state=0;
					else sel_state++;
			}
	}
	else if(step <0)
	{
		  if(scr==screen_0)
			{
		   if(sel_state==0)sel_state=1;
		   else sel_state--;
			}
			if(scr==screen_1)
			{
		   if(sel_state==0)sel_state=4;
		   else sel_state--;
			}
			if(scr==screen_2)
			{
				if(sel_state==0)sel_state=2;
					else sel_state--;
			}
	}
}
/*********************************************************
 *函数名称：change_screen
 *函数功能：
 *输入参数：step
 *输出参数：无
 *********************************************************/

//---------------------------------------------------------
void change_screen(unsigned int scr)
{
	
}
/*********************************************************
 *函数名称：app_running
 *函数功能：显示函数总程序
 *输入参数：sel
 *输出参数：无
 *********************************************************/
//---------------------------------------------------------
void app_running(unsigned int scr,unsigned int sel)
{
	if(scr==screen_0)
	{
		if(sel==0)
		{
			scr_state=screen_1;
			sel_state=0;
			lcd_clear(White);
		  screen_display(scr_state,sel_state,0);
			syn6288_SpeakStr("手动操作",0);
		}
	}
	if(scr==screen_1)
	{
		scr_state=screen_2;
		lcd_clear(White);
		screen_display(scr_state,0,0);
		
		DAC_Cmd(DAC_Channel_1, ENABLE);
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	  TIM_Cmd(TIM7, ENABLE);
			switch(sel)
		{
			case 0:
			wave_pattern=PATTERN_MASSAGE;
      syn6288_SpeakStr("按摩",0);		
			break;
			case 1:
	    wave_pattern=PATTERN_STROKE;
			syn6288_SpeakStr("锤击",0);
			break;
			case 2:
	    wave_pattern=PATTERN_CUPPING;
			syn6288_SpeakStr("拔罐",0);
			break;
			case 3:
	    wave_pattern=PATTERN_ACUPUNCTURE;
			syn6288_SpeakStr("针灸",0);
			break;
			case 4:
	    wave_pattern=PATTERN_SCRAPPING;
			syn6288_SpeakStr("刮痧",0);
			break;
			default:break;
		}
		sel_state=0;
	}
	
}
//---------------------------------------------------------
/*********************************************************
 *函数名称：key_select
 *函数功能：显示控制函数，上下移动，确认退出
 *输入参数：key,
 *输出参数：无
 *********************************************************/
//---------------------------------------------------------
void key_select(unsigned int scr,unsigned int key)
{
	switch(key)
	{
		case KEY_UP:
				change_sel(scr,-1);
				screen_display(scr,sel_state,0);
				break;
		case KEY_DOWN:
				change_sel(scr,1);
				screen_display(scr,sel_state,0);
				break;
		case KEY_OK:
				app_running(scr_state,sel_state);
				break;
		case KEY_CANCEL:
			/*********波形模式，光标锁定位置零*********/
			  wave_pattern=0;
		    sel_state=0;
		    if(scr==screen_1)
				{
					scr_state=screen_0;
					lcd_clear(White);
					screen_display(scr_state,sel_state,0);
					syn6288_SpeakStr("退出",0);
				}
				/************如果为屏幕二退回屏幕一DDS频率调为最低频强度调为0*********/
			  if(scr==screen_2)
				{
					scr_state=screen_1;
					DAC_Cmd(DAC_Channel_1, DISABLE);
					TIM_ITConfig(TIM7,TIM_IT_Update,DISABLE);
	        TIM_Cmd(TIM7, DISABLE);
		      DDS_step=0;
					amplitude_level=0;
		      DDSM=1;
					time_interval=5;
					tim7_counter1=0;
					tim7_counter2=0;
					lcd_clear(White);
					screen_display(scr_state,sel_state,0);
					syn6288_SpeakStr("退出",0);
				}
			  break;	
		case KEY_LEFT:
				 if(scr==screen_2)
				 {
					 switch (sel_state)
					 {
						 case 0:
							 if(amplitude_level>0)
						 {
						  amplitude_level--;
							value_display(16,90,amplitude_level);
							syn6288_SpeakStr("强度减",0);
						 }
							 break;	
             case 1:
							 if(DDSM>1)
						 {
						  DDSM--; 
							value_display(16,150,DDSM);
							syn6288_SpeakStr("频率减",0);
						 }
							 break;
             case 2:
							 if(time_interval>1)
						 {
							tim7_counter1=0;
					    tim7_counter2=0;
						  time_interval--;
							value_display(16,210,time_interval);
							syn6288_SpeakStr("间隔减",0);
						 }
							 break;
             default:break;							 
					 } 
				 }
				 break;	 
		case KEY_RIGHT:
				 if(scr==screen_2)
				 { 
					 switch (sel_state)
					 {
						 case 0:
							if(amplitude_level<6)
						 {
						  amplitude_level++;
							value_display(16,90,amplitude_level);
							syn6288_SpeakStr("强度加",0); 
						 }
							 break;	
             case 1:
							 if(DDSM<4)
						 {
						  DDSM++;
						  value_display(16,150,DDSM);
							syn6288_SpeakStr("频率加",0); 
						 }
							 break;
             case 2:
							 if(time_interval<5)
						 {
							tim7_counter1=0;
					    tim7_counter2=0;
						  time_interval++;
							value_display(16,210,time_interval);
							syn6288_SpeakStr("间隔加",0);
						 }
							 break;
             default:break;							 
					 } 
				 }
				 break;
	  default:break;
	}
}



 /*********************************************************
 *函数名称：screen_display
 *函数功能：屏幕显示函数
 *输入参数：sel 表示锁定到哪一行,并且为按键是否检测处理完毕的标志位
 *输出参数：key_entry 上下左右键值
 *********************************************************/

void screen_display(unsigned int scr,unsigned int sel,unsigned int key_entry)
{
    int font_num;
	if(sel !=DETECT_KEY)
	{
		if(scr==screen_0)
		{
			for(font_num=0;font_num<2;font_num++)
			{ 
				if(font_num==0)
				LCD_write_ASCII16x24_string(70,4+48*(font_num+2),pstr[font_num],(sel==font_num)?Red:Green,White);
				if(font_num==1)
				LCD_write_ASCII16x24_string(48,4+48*(font_num+2),pstr[font_num],(sel==font_num)?Red:Green,White);
			}
		  
		}	
		if(scr==screen_1)	
		{
			for(font_num=0;font_num<5;font_num++)
		  LCD_write_ASCII16x24_string(16,4+48*(font_num+1),pstr[font_num+2],(sel==font_num)?Red:Green,White);
		}
		if(scr==screen_2)	
		{
			LCD_write_ASCII8x16_string(16,4,pchar10,Magenta,White);
			for(font_num=0;font_num<3;font_num++)
		  LCD_write_ASCII16x24_string(16,4+60*(font_num+1),pstr[font_num+7],(sel==font_num)?Red:Green,White);
			value_display(16,90,amplitude_level);
			value_display(16,150,DDSM);
			value_display(16,210,time_interval);
		}
	}
	else
	{
		key_select(scr,key_entry);
	}
}




