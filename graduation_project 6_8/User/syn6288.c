#include "syn6288.h"

char syn_state=0;
void syn_Delaynms(unsigned char n);

void syn6288_SpeakStr(char *text,char MusicID)	//MusicID为需要播放的背景音乐变化，0不播放  
{
	char FrameHead[5];								 
	int  cnt=0; 
	char length=0;                 
	char check=0;	                

	length=strlen(text);           

	FrameHead[0]=0xfd;            		   
	FrameHead[1]=0x00;		   
	FrameHead[2]=length+3;	   
	FrameHead[3]=01;			   
	FrameHead[4]=00+(MusicID<<3);	//GB2312	MusicID=0无背景音乐=1有背景音乐		   
  
	for(cnt=0;cnt<5;cnt++)			   
	{
		check=check^(FrameHead[cnt]);   	
		USART_SendData(USART2,FrameHead[cnt]);
	  while (!(USART2->SR & USART_FLAG_TXE));
	}	
	   
	for(cnt=0;cnt<length;cnt++)		    
	{

		check=check^(*text);			   	
		USART_SendData(USART2,*text);
	  while (!(USART2->SR & USART_FLAG_TXE));		
		text++;
	}
  USART_SendData(USART2,check);
	while (!(USART2->SR & USART_FLAG_TXE));		
	syn_state=1;
	while(syn_state);
	syn_Delaynms(8);
	
}

void syn_Delaynms(unsigned char n)
{
	int x;
	while(n--)
	{
		for(x=0;x<20000;x++)
		{		
		}		
	}

}
void syn_Release()
{
syn_state=0;//idle
}
