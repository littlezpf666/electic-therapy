#include "output_wave.h"

extern uint16_t DDSM;
//波形模式，幅度，间隔时间
char wave_pattern=0,amplitude_level=0,time_interval=5;
void massage_running()
{
	
	wave_pattern=PATTERN_MASSAGE;
}

