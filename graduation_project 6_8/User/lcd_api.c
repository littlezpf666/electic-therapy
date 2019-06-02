#include "lcd_api.h"
#include "ili_lcd_general.h"

extern const char ASCII8x16_Table[][16];
extern const uint16_t ASCII16x24_Table[][24];           
void LCD_write_ASCII8x16(uint16_t x,uint16_t y,uint8_t str,unsigned int color,unsigned int xcolor)//写字符
{
    uint16_t xpos = x;
    uint16_t ypos = y;

    unsigned char avl,i,n;

    for(i=0; i<16; i++) //16行
    {
        avl= (ASCII8x16_Table[str-32][i]);
			  
        lcd_SetCursor(xpos,ypos);
        ypos++;
        rw_data_prepare();
        for(n=0; n<8; n++) //8列
        {
            if(avl&0x80) write_data(color);
            else write_data(xcolor);
            avl<<=1;
        }
    }
}

void LCD_write_ASCII16x24(uint16_t x,uint16_t y,uint8_t str,unsigned int color,unsigned int xcolor)//写字符
{
	uint16_t xpos = x;
  uint16_t ypos = y;
	uint16_t avl= 0x0000;
	unsigned char i,n;

    for(i=0; i<24; i++) //24行
    {
        avl= ASCII16x24_Table[str-32][i];
        lcd_SetCursor(xpos,ypos);
        ypos++;
        rw_data_prepare();
        for(n=0; n<16; n++) //16列
        {
            if(avl&0x0001) write_data(color);
            else write_data(xcolor);
            avl>>=1;
        }
    }
}


void LCD_write_ASCII8x16_string(uint16_t x,uint16_t y,char *s,unsigned int color,unsigned int xcolor)//英文字符串显示
{
    while (*s)
    {
        LCD_write_ASCII8x16(x,y,*s,color,xcolor);
        s++;
        x += 8;
    }
}

void LCD_write_ASCII16x24_string(uint16_t x,uint16_t y,char *s,unsigned int color,unsigned int xcolor)//英文字符串显示
{
    while (*s)
    {
        LCD_write_ASCII16x24(x,y,*s,color,xcolor);
        s++;
        x += 16;
    }
}

/*------------------------数值显示--------------------------------*/
void value_display(unsigned char x,uint16_t y,unsigned int value)
{
   unsigned char num[]="0123456789";
   LCD_write_ASCII16x24(x,y,num[value%100/10],Blue,White);
   LCD_write_ASCII16x24(x+16,y,num[value%10],Blue,White);
}

void LCD_draw_raw_line(uint16_t x,uint16_t y,unsigned int color,unsigned int xcolor)
{
	  uint16_t xpos = x;
    uint16_t ypos = y;
	  unsigned char n;
    lcd_SetCursor(xpos,ypos);
		rw_data_prepare();
		for(n=0; n<240; n++) //239列
		{
				write_data(color);
		}
}


//void LCD_draw_column_line(uint16_t x,uint16_t y,unsigned int color,unsigned int xcolor)
//{
//	  uint16_t xpos = x;
//    uint16_t ypos = y;
//	  unsigned char n;
//    
//		rw_data_prepare();
//		for(n=0; n<240; n++) //239列
//		{
//			  lcd_SetCursor(xpos,ypos);
//				write_data(color);
//			  ypos++; 
//		}
//}
