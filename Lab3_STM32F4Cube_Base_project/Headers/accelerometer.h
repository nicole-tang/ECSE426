#ifndef _ACCELEROMETER_H
#define _ACCELEROMETER_H

#include "stm32f4xx_hal.h"



void initialize_accel(void);
void reading_accel_values(float *ax, float *ay, float *az);
float tilt_angle(float ax, float ay, float az);


	
#endif