#include "stm32f4xx_hal.h"
#include "math.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "display.h"

// Mode is changed to temp_mode when '#' is pressed during acc_mode

void Thread_display (void const *argument);                 // thread function
osThreadId tid_Thread_display;                              // thread id
osThreadDef(Thread_display, osPriorityNormal, 1, 0);

int mode, submode, flash;

int start_Thread_display(void)
{
	tid_Thread_display = osThreadCreate(osThread(Thread_display), NULL);
	if(!tid_Thread_display) return (-1);
	return (0);
}

void Thread_display(void const *argument)
{
	
}

void seven_segment_flash(){
}
void seven_segment_stop_flash(){
}
void seven_segment_set_temp(float filtered_temp){
}

