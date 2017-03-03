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
TIM_OC_InitTypeDef TIM_OCHandle;
int TIM_flag;

void initialize_timer(){
	//enable the timer clock
	__TIM4_CLK_ENABLE();
	
	
	//Create the Timer struct and fill it with configuration options 
	/*
		TIM4 is connected to APB1 bus, has 42MHz clock
		timer has interal PLL, double the frequency = 84MHz
		https://stm32f4-discovery.net/2014/05/stm32f4-stm32f429-discovery-pwm-tutorial/
	
		Tutorial 3 Slide 11
		Desired timer frequency  = timer input frequency/(prescalar*period)
		input frequency = 84MHz (APB1 timer clocks)
		Desired timer frequency = 2000Hz (from lab requirement)
	
		2000Hz = 84MHz/(prescalar*period)
		prescalar*period = 84MHz/2000Hz = 42000
		since 42000 is less than 2^16=65536, choose prescaler to 1 and period to 65536
	*/
	TIM_Handle.Instance = TIM4;																// general purpose timer for PWM generation; 4 capture channels; 16 bits, APB1 42MHz (max)
	TIM_Handle.Init.Prescaler = 1;														// Specifies the prescaler value used to divide the TIM clock
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;					// timer counts up until the timer period is reached. Then the timer is reset.
	TIM_Handle.Init.Period = 42000;																// period does not exceed 65535 (2^16-1) because TIM3 is 16 bit
	TIM_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;			// no division
	TIM_Handle.Init.RepetitionCounter=0;											// no repetition count
	
	
	// initialize TIM4
	HAL_TIM_Base_Init(&TIM_Handle);
	// Start count on TIM4
	HAL_TIM_Base_Start(&TIM_Handle);	
	
}

void initialize_PWM(){
	
	// PWM1 is set on compare match; PWM2 is clear on compare match
	TIM_OCHandle.OCMode = TIM_OCMODE_PWM1;
	TIM_OCHandle.OCPolarity = TIM_OCPOLARITY_LOW;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
