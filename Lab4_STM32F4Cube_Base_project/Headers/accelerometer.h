#ifndef _ACCELEROMETER_H
#define _ACCELEROMETER_H

/* Includes -------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <math.h>
#include "lis3dsh.h"
#include <stdint.h>
#include "cmsis_os.h"  

#define PI 3.141592653589793
#define RAD_TO_DEG(x) ((x*180.0) / PI)


void Thread_accelerometer(void const *argument);
void deinitialize_GPIO_button(void);
void initialize_GPIO_led_lights_PWM(void);
void initialize_accel(void);
void EXTI0_IRQHandler(void);
void reading_accel_values(float *acc);
float pitch_tilt_angle(float *acc);
float roll_tilt_angle(float *acc);
void calibration_accel(float *acc);
int start_Thread_accelerometer(void);


extern int pitch_output, roll_output, mode;
	
#endif
