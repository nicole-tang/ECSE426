/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
 Define to prevent recursive inclusion -------------------------------------*/

#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx_hal.h"
#include "main.h"
#include "cmsis_os.h"  

void Thread_ADC (void const *argument); 
float tempConversion(float voltage);
int FIR_C(float* InputArray, float* OutputArray,float* coeff, int Length, int Order);
int start_Thread_ADC (void);
void initialize_ADC(void);

extern float filtered_temp;
extern osThreadId tid_Thread_ADC;   
#endif





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
