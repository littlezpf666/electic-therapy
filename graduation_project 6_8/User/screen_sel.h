#ifndef SCREEN_SEL_H
#define SCREEN_SEL_H

#include "ili_lcd_general.h"
#include "lcd_api.h"
#include "key.h"
#include "syn6288.h"

#define DETECT_KEY 255
#define screen_0   0
#define screen_1   1
#define screen_2   2

#define PATTERN_MASSAGE 1
#define PATTERN_STROKE 2
#define PATTERN_CUPPING 3
#define PATTERN_ACUPUNCTURE 4
#define PATTERN_SCRAPPING 5

void screen_display(unsigned int scr,unsigned int sel,unsigned int key_entry);

#endif