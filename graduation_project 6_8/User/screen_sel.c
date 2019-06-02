/**********************��Ļ��ʾ����****************************/
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
//���ñ���Ҫ��Դ����������������ͬ
extern unsigned int tim7_counter1,tim7_counter2;
extern uint16_t DDS_step,DDSM;	
/*********************************************************
 *�������ƣ�change_sel
 *�������ܣ������ƶ�����ѡ�й�꣩
 *���������step
 *�����������
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
 *�������ƣ�change_screen
 *�������ܣ�
 *���������step
 *�����������
 *********************************************************/

//---------------------------------------------------------
void change_screen(unsigned int scr)
{
	
}
/*********************************************************
 *�������ƣ�app_running
 *�������ܣ���ʾ�����ܳ���
 *���������sel
 *�����������
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
			syn6288_SpeakStr("�ֶ�����",0);
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
      syn6288_SpeakStr("��Ħ",0);		
			break;
			case 1:
	    wave_pattern=PATTERN_STROKE;
			syn6288_SpeakStr("����",0);
			break;
			case 2:
	    wave_pattern=PATTERN_CUPPING;
			syn6288_SpeakStr("�ι�",0);
			break;
			case 3:
	    wave_pattern=PATTERN_ACUPUNCTURE;
			syn6288_SpeakStr("���",0);
			break;
			case 4:
	    wave_pattern=PATTERN_SCRAPPING;
			syn6288_SpeakStr("����",0);
			break;
			default:break;
		}
		sel_state=0;
	}
	
}
//---------------------------------------------------------
/*********************************************************
 *�������ƣ�key_select
 *�������ܣ���ʾ���ƺ����������ƶ���ȷ���˳�
 *���������key,
 *�����������
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
			/*********����ģʽ���������λ����*********/
			  wave_pattern=0;
		    sel_state=0;
		    if(scr==screen_1)
				{
					scr_state=screen_0;
					lcd_clear(White);
					screen_display(scr_state,sel_state,0);
					syn6288_SpeakStr("�˳�",0);
				}
				/************���Ϊ��Ļ���˻���ĻһDDSƵ�ʵ�Ϊ���Ƶǿ�ȵ�Ϊ0*********/
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
					syn6288_SpeakStr("�˳�",0);
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
							syn6288_SpeakStr("ǿ�ȼ�",0);
						 }
							 break;	
             case 1:
							 if(DDSM>1)
						 {
						  DDSM--; 
							value_display(16,150,DDSM);
							syn6288_SpeakStr("Ƶ�ʼ�",0);
						 }
							 break;
             case 2:
							 if(time_interval>1)
						 {
							tim7_counter1=0;
					    tim7_counter2=0;
						  time_interval--;
							value_display(16,210,time_interval);
							syn6288_SpeakStr("�����",0);
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
							syn6288_SpeakStr("ǿ�ȼ�",0); 
						 }
							 break;	
             case 1:
							 if(DDSM<4)
						 {
						  DDSM++;
						  value_display(16,150,DDSM);
							syn6288_SpeakStr("Ƶ�ʼ�",0); 
						 }
							 break;
             case 2:
							 if(time_interval<5)
						 {
							tim7_counter1=0;
					    tim7_counter2=0;
						  time_interval++;
							value_display(16,210,time_interval);
							syn6288_SpeakStr("�����",0);
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
 *�������ƣ�screen_display
 *�������ܣ���Ļ��ʾ����
 *���������sel ��ʾ��������һ��,����Ϊ�����Ƿ��⴦����ϵı�־λ
 *���������key_entry �������Ҽ�ֵ
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




