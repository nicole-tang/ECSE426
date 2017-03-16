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

void initialize_accel(void);
void reading_accel_values(float *acc);
float pitch_tilt_angle(float *acc);
float roll_tilt_angle(float *acc);
void calibration_accel(float *acc);

int start_Thread_acceleration(void);
void Thread_acceleration(void const *argument);

extern int pitch_output, roll_output;
	
#endif