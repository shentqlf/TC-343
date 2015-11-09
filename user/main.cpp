/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "led.h"
#include "button.h"

#include "time-protocol.h"
#include "clock.h"

#define PPS_PIN PA7

LED led(&PA6,&PA4,&PA5);
BUTTON btn(&PA0,1);

void pps_on()
{

	PPS_PIN.set();
}
void pps_off()
{
	PPS_PIN.reset();
}

u8 y = 0;
void setup()
{
	ebox_init();

	PPS_PIN.mode(OUTPUT_PP);
	pps_off();
	uart1.begin(115200);
	led.begin();
	btn.begin();
	date_time.begin(115200);

}
u8 disp_buf[100];
u8 date_time_string[14] = "3010152141001";
u8 cutdown_string[7] = "000000";
u8 play_time_string[7] = "000000";
uint32_t x;
u32 current_systick = 0;

#include "string.h"
int j = 0;

int main(void)
{
	setup();
	while(1)
	{
		

		if(date_time.process() == 1)
		{
			clock.set_date_time(date_time.date_time);
			memcpy(date_time_string,date_time.date_time,14);
			memcpy(play_time_string,date_time.playtime_string,6);
			cutdown_string[0] = date_time.cutdown_string[3];
			cutdown_string[1] = date_time.cutdown_string[4];
			cutdown_string[2] = date_time.cutdown_string[5];
			cutdown_string[3] = date_time.cutdown_string[6];
			cutdown_string[4] = date_time.cutdown_string[7];
			cutdown_string[5] = date_time.cutdown_string[8];
			cutdown_string[6] = 0;
			date_time_string[12] = clock.dt.week;
			
			j = 0;
			for(int i = 0; i < 6; i++)
			{
				disp_buf[j++] = play_time_string[5 - i];
			}			
			for(int i = 0; i < 6; i++)
			{
				disp_buf[j++] = cutdown_string[5 - i];
			}
			for(int i = 0; i < 6; i++)
			{
				disp_buf[j++] = date_time_string[11 - i];
			}
			led.show_date_time(disp_buf);
			current_systick = millis();
		}
		
		if(millis() - current_systick > 500)
			pps_off();
		else
			pps_on();
	}


}




