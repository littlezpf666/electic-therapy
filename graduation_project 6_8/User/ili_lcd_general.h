#ifndef ILI_LCD_GENERAL_H_INCLUDED
#define ILI_LCD_GENERAL_H_INCLUDED

/*
 Compatible list:
 ili9320 ili9325 ili9328
 LG4531
*/

/* LCD color */
#define White            0xFFFF
#define Black            0x0000
#define Grey             0xF7DE
#define Blue             0x001F
#define Blue2            0x051F
#define Red              0xF800
#define Magenta          0xF81F
#define Green            0x07E0
#define Cyan             0x7FFF
#define Yellow           0xFFE0

/*---------------------- Graphic LCD size definitions ------------------------*/
#define LCD_WIDTH       320                 /* Screen Width (in pixels)           */
#define LCD_HEIGHT      240                 /* Screen Hight (in pixels)           */
#define BPP             16                  /* Bits per pixel                     */
#define BYPP            ((BPP+7)/8)         /* Bytes per pixel                    */

//LCD初始化
extern void lcd_Initializtion(void);
//设置坐标
extern void lcd_SetCursor(unsigned int x,unsigned int y);
//获取LCD的ID
extern unsigned int lcd_getdeviceid(void);
//清屏
extern void lcd_clear(unsigned short Color);
extern void rw_data_prepare(void);
extern void write_data(unsigned short data );
//#define _ILI_REVERSE_DIRECTION_ //如果需要上下倒屏,请打开此项
//#define use_rt_gui              //如果需要在RTGUI中使用,请打开此项


#if defined(use_rt_gui)
#include "rtthread.h"
#include <rtgui/rtgui.h>
#include <rtgui/driver.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
extern void rt_hw_lcd_update(rtgui_rect_t *rect);
extern rt_uint8_t * rt_hw_lcd_get_framebuffer(void);
extern void rt_hw_lcd_set_pixel(rtgui_color_t *c, rt_base_t x, rt_base_t y);
extern void rt_hw_lcd_get_pixel(rtgui_color_t *c, rt_base_t x, rt_base_t y);
extern void rt_hw_lcd_draw_hline(rtgui_color_t *c, rt_base_t x1, rt_base_t x2, rt_base_t y);
extern void rt_hw_lcd_draw_vline(rtgui_color_t *c, rt_base_t x, rt_base_t y1, rt_base_t y2);
extern void rt_hw_lcd_draw_raw_hline(rt_uint8_t *pixels, rt_base_t x1, rt_base_t x2, rt_base_t y);
#endif

#endif // ILI_LCD_GENERAL_H_INCLUDED
