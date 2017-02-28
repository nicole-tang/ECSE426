/**
  ******************************************************************************
  * File Name          : timer.c
  * Description        : This file provides code for the configuration
  *                      of timer.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "main.h"
#include "stm32f4xx_hal.h"
/* USER CODE BEGIN 0 */
TIM_HandleTypeDef TIM_Handle;
int TIM_flag;

void initialize_timer(){
	//enable the timer clock
	__TIM3_CLK_ENABLE();
	
	
	//Create the Timer struct and fill it with configuration options 
	/*
		Tutorial 3 Slide 11
		Desired timer frequency  = timer input frequency/(prescalar*period)
		input frequency = 84MHz (APB1 timer clocks)
		Desired timer frequency = 1000Hz (1ms period)
		scalar*period = 84MHz/1000Hz = 84000Hz
	*/
	TIM_Handle.Instance = TIM3;																// general purpose timer for PWM generation; 4 capture channels; 16 bits, APB1 42MHz (max)
	TIM_Handle.Init.Prescaler = 1000;															// Specifies the prescaler value used to divide the TIM clock.
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;					// timer counts up until the timer period is reached. Then the timer is reset.
	TIM_Handle.Init.Period = 84;																// period does not exceed 65535 (2^16-1) because TIM3 is 16 bit
	
	HAL_TIM_Base_Init(&TIM_Handle);
	HAL_TIM_Base_Start_IT(&TIM_Handle);	
	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
