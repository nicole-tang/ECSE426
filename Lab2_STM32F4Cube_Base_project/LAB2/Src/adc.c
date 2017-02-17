/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

ADC_HandleTypeDef ADC1_Handle;

/* USER CODE BEGIN 0 */
// Initialize ADC
void initialize_ADC(void){
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	ADC_InitTypeDef ADC_init;
	ADC_ChannelConfTypeDef channelConfig;
	
	/*First struct ADC_InitTypeDef*/
	ADC_init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;     /*Select the frequency of the clock to the ADC*/
	ADC_init.Resolution = ADC_RESOLUTION_12B;               /*Choose resolution to be 12 bits, slower but more precision*/
	ADC_init.DataAlign =  ADC_DATAALIGN_RIGHT;              /*Data alignment is right */
	ADC_init.ScanConvMode = DISABLE;                        /*One channel mode, we don't need multi channel*/
	ADC_init.EOCSelection = ADC_EOC_SEQ_CONV;               /*Perform ADC conversions without having to read all conversion data; whether the EOC flag is set 
																														at the end of single channel conversion or at the end of all conversions.*/ 
	ADC_init.ContinuousConvMode = DISABLE;									/*Specifies whether the conversion is performed in Continuous or Single mode */
	ADC_init.DMAContinuousRequests = DISABLE;               /*DMA request is not performed*/
	ADC_init.NbrOfConversion = 1;                           /*number of ADC conversions that will be done using sequencer for regular channel group */
	ADC_init.DiscontinuousConvMode = ENABLE;								/*Specifies whether the conversion is performed in Discontinuous or not 
																														for regular channels.*/
	ADC_init.ExternalTrigConv = ADC_SOFTWARE_START;         /*Disable external trigger*/
	ADC_init.ExternalTrigConvEdge = ADC_SOFTWARE_START;     /*Disable external trigger*/
	
	/*Second struct ADC_HandleTypeDef*/
	ADC1_Handle.Instance = ADC1;                           
	ADC1_Handle.Init = ADC_init;
	ADC1_Handle.NbrOfCurrentConversionRank = 1;
	ADC1_Handle.State = 0;
	ADC1_Handle.ErrorCode = HAL_ADC_ERROR_NONE;
	
	/*Third struct ADC_ChannelConfTypeDef*/
	channelConfig.Channel = ADC_CHANNEL_16; 
	channelConfig.Rank = 1;
	channelConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	channelConfig.Offset = 0;
	
	HAL_ADC_ConfigChannel(&ADC1_Handle, &channelConfig);
	HAL_ADC_Init(&ADC1_Handle);
	
}

float function_ADC(void){
	float voltage = 0.0;
	// HAL_ADC_Start starts ADC conversions when the polling method is used
	HAL_ADC_Start(&ADC1_Handle);
	if((HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK)){
		voltage = HAL_ADC_GetValue(&ADC1_Handle);
	}
	HAL_ADC_Stop(&ADC1_Handle);
	// resolution is in 12 bits (4096 = 2^12) with Vref = 3V
	return (voltage*3.0)/4096.0; 
}
	
/* USER CODE END 0 */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
