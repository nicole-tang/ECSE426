#include "stm32f4xx_hal.h"
#include "math.h"
#include "cmsis_os.h"
#include "adc.h" 
#include "display.h"

#define HEAT_THRESHOLD 60
#define FLASH_INTERVAL 70


void Thread_display (void const *argument);                 // thread function
osThreadId tid_Thread_display;                              // thread id
osThreadDef(Thread_display, osPriorityNormal, 1, 0);


int start_Thread_display(void)
{
	tid_Thread_display = osThreadCreate(osThread(Thread_display), NULL);
	if(!tid_Thread_display) return (-1);
	return (0);
}

void Thread_display(void const *argument)
{
	int is_flashing, flash_counter, flash_on, flash_off;
	
	flash_counter = 0;
	is_flashing = 0;
	flash_on = FLASH_INTERVAL;
	flash_off = FLASH_INTERVAL * 2;
	
	while(1)
	{
		if(filtered_temp > HEAT_THRESHOLD)
		{
			is_flashing = 1;
		} else {
			is_flashing = 0;
		}
		
		if(is_flashing == 1)
		{
			if(mode == 1)
			{
				if(submode == 0) 
				{
					//display pitch angle

				}
		
				if(submode == 1)
				{
					//display roll angle
				}
			}
	
			if(mode == 2)
			{
				
			}
		}
		
		if(is_flashing == 0)
		{
			if(mode == 1)
			{
			if(submode == 0) 
				{
					//display pitch angle

				}
		
				if(submode == 1)
				{
					//display roll angle
				}
			}
			
			if(mode == 2)
			{
			
			}
				
		}
	}
}

void seven_segment_flash(){
	
}

void seven_segment_stop_flash(){
}

void seven_segment_set_temp(float filtered_temp){
}

