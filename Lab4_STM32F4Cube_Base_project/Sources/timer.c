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
#include "display.h"
#include "cmsis_os.h" 

/* USER CODE BEGIN 0 */
TIM_HandleTypeDef TIM3_Handle;
TIM_HandleTypeDef TIM4_Handle;
TIM_OC_InitTypeDef TIM_OCHandle;
int counter4=0;
int counter10=0;

void initialize_timer(void){
	//enable the timer clock
		__HAL_RCC_TIM4_CLK_ENABLE();
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
	TIM4_Handle.Instance = TIM4;																// general purpose timer for PWM generation; 4 capture channels; 16 bits, APB1 42MHz (max)
																														// TIM4's output channel is connected to PD12-15 (the LEDs)
	TIM4_Handle.Init.Prescaler = 100;														// Specifies the prescaler value used to divide the TIM clock
	TIM4_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;					// timer counts up until the timer period is reached. Then the timer is reset.
	TIM4_Handle.Init.Period = 420;														// period does not exceed 65535 (2^16-1) because TIM3 is 16 bit
	//TIM4_Handle.Channel=TIM_CHANNEL_ALL;
	TIM4_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter = 0;
	
	// initialize TIM4
	HAL_TIM_PWM_Init(&TIM4_Handle);
	TIM_OCHandle.Pulse = 0;																		// Specifies the pulse value to be loaded into the Capture Compare Register. 
	TIM_OCHandle.OCMode = TIM_OCMODE_PWM1;										// Specifies the TIM mode
																														// PWM1 is set on compare match; PWM2 is clear on compare match
	TIM_OCHandle.OCPolarity = TIM_OCPOLARITY_HIGH;						// Specifies the output polarity
																														// the output begins with HIGH and switches to LOW
	TIM_OCHandle.OCFastMode = TIM_OCFAST_DISABLE;							// Specifies the Fast mode state.

	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OCHandle,TIM_CHANNEL_1);					//For green LED
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OCHandle,TIM_CHANNEL_2);					//For orange LED
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OCHandle,TIM_CHANNEL_3);					//For red LED
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OCHandle,TIM_CHANNEL_4);					//For blue LED

	HAL_TIM_PWM_Start(&TIM4_Handle,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM4_Handle,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&TIM4_Handle,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&TIM4_Handle,TIM_CHANNEL_4);
	/*--Initialize another timer------------------------*/
	__HAL_RCC_TIM3_CLK_ENABLE();	
	TIM3_Handle.Instance = TIM4;																// general purpose timer for PWM generation; 4 capture channels; 16 bits, APB1 42MHz (max)
																														// TIM4's output channel is connected to PD12-15 (the LEDs)
	TIM3_Handle.Init.Prescaler = 100;														// Specifies the prescaler value used to divide the TIM clock
	TIM3_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;					// timer counts up until the timer period is reached. Then the timer is reset.
	TIM3_Handle.Init.Period = 840;														// period does not exceed 65535 (2^16-1) because TIM3 is 16 bit
	TIM3_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM3_Handle.Init.RepetitionCounter = 0;
	
	HAL_TIM_Base_Init(&TIM3_Handle);
	HAL_TIM_Base_Start_IT(&TIM3_Handle);
	HAL_NVIC_SetPriority(TIM3_IRQn,0,1); //highest priority
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
}
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handle);
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM4_Handle);
}

void change_pulse(int degree_difference,uint32_t Channel){
	//the percentage of degree difference to period determines the pulse
	TIM_OCHandle.Pulse = (degree_difference/180.0)*420;
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OCHandle,Channel);		
	HAL_TIM_PWM_Start(&TIM4_Handle,Channel);	
}

// to turn off led
void turn_off_led(uint32_t Channel){
	HAL_TIM_PWM_Stop(&TIM4_Handle, Channel);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// 1000hz for display
	osSignalSet(tid_Thread_display, 0x4);
	if (counter4 == 4)
	{
		// 250hz for display flash
		osSignalSet(tid_Thread_display, 0x5);
		counter4 = 0;
	}
	counter4++;
	if (counter10 == 10)
	{
		// 100hz for keypad
		osSignalSet(tid_Thread_keypad, 0x3);
		// 100hz for ADC
		osSignalSet(tid_Thread_ADC, 0x1);
		counter10 = 0;
	}
	counter10++;	
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
