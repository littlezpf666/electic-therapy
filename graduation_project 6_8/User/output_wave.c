#include "output_wave.h"

extern uint16_t DDSM;
//����ģʽ�����ȣ����ʱ��
char wave_pattern=0,amplitude_level=0,time_interval=5;
void massage_running()
{
	
	wave_pattern=PATTERN_MASSAGE;
}

