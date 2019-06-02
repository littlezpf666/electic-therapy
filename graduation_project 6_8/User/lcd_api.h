#ifndef LCD_API_H_INCLUDED
#define LCD_API_H_INCLUDED

#include <stdint.h>

extern void LCD_write_ASCII8x16(uint16_t x,uint16_t y,uint8_t str,unsigned int color,unsigned int xcolor);//写字符
extern void LCD_write_ASCII8x16_string(uint16_t x,uint16_t y,char *s,unsigned int color,unsigned int xcolor);//英文字符串显示

extern void LCD_write_ASCII16x24(uint16_t x,uint16_t y,uint8_t str,unsigned int color,unsigned int xcolor);//写字符
extern void LCD_write_ASCII16x24_string(uint16_t x,uint16_t y,char *s,unsigned int color,unsigned int xcolor);//英文字符串显示

extern void value_display(unsigned char x,uint16_t y,unsigned int value);//数值显示

extern void LCD_draw_raw_line(uint16_t x,uint16_t y,unsigned int color,unsigned int xcolor);//画直线
#endif // LCD_API_H_INCLUDED
