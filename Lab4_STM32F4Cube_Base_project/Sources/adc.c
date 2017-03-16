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
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "display.h"

#define TEMPERATURETHRESHOLD 30
#define DISPLAYINGCOUNTER 100

// For thread
void Thread_ADC (void const *argument);                 // thread function
osThreadId tid_Thread_ADC;                              // thread id
osThreadDef(Thread_ADC, osPriorityHigh, 1, 0);

ADC_HandleTypeDef ADC1_Handle;

// Conversion from voltage to celsius
float tempConversion(float voltage){
	float V_25 = 0.76;
	float avg_slope = 2.5/1000;
	return ((voltage-V_25)/avg_slope)+25;
}

int FIR_C(float* InputArray, float* OutputArray,float* coeff, int Length, int Order){
	//for all sample in the InputArray
	for(int n=0;n<Length-Order-1;n++){
		//temp variable to store the accumulative sum of the filter
		float sum=0;
		//iterate for the number of existing coefficients
		for(int b=0;b<=Order;b++){
			//multiply the content of pointer coeff to content of point input array
			sum+= InputArray[n+b] * coeff[b];
		}		
		//store the result
		OutputArray[n] = sum;
	}
	return 0;
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_ADC (void) {
  tid_Thread_ADC = osThreadCreate(osThread(Thread_ADC ), NULL); // Start ADC_Thread
  if (!tid_Thread_ADC) return(-1); 
  return(0);
}


 /*----------------------------------------------------------------------------
*      Thread  'LED_Thread': Toggles LED
 *---------------------------------------------------------------------------*/
	void Thread_ADC (void const *argument) {
		float voltage = 0.0;
		float voltage_temp = 0.0;
		float temperature=0.0;
		float filtered_temp=0.0;
		float coeff[]={0.6,0.5,0.01,0.3,0.5};
		int displaying_count=0;
			
		
		
		osEvent event;
		while(1){
			//wait for an infinite time until the signal from ADC(0x1) is set.
			event = osSignalWait(0x1, osWaitForever);
			//osEventSignal: signal occurred, value.signals contains the signal flags
			//these signal flags are cleared.
			if((event.status == osEventSignal) && (HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK)){
				voltage = HAL_ADC_GetValue(&ADC1_Handle);
				voltage_temp=(voltage*3.0)/4096.0;
				temperature=tempConversion(voltage_temp);
				
				FIR_C(&temperature, &filtered_temp,coeff,1,(sizeof coeff/sizeof(float))-1);
				
				if(temperature>TEMPERATURETHRESHOLD){
					seven_segment_flash();
				}else if(temperature<TEMPERATURETHRESHOLD){
					seven_segment_stop_flash();	
				}
				
				// display only when time counter hits the number of displaying counter 
			if(displaying_count++ >= DISPLAYINGCOUNTER){
				//reset displaying_count
				displaying_count=0;
				seven_segment_set_temp(filtered_temp);				
			}
				
			}
	}
}

/*----------------------------------------------------------------------------
 *      Initialize ADC
 *---------------------------------------------------------------------------*/
	
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
	ADC_init.ContinuousConvMode = ENABLE;									/*Specifies whether the conversion is performed in Continuous or Single mode */
	ADC_init.DMAContinuousRequests = DISABLE;               /*DMA request is not performed*/
	ADC_init.NbrOfConversion = 1;                           /*number of ADC conversions that will be done using sequencer for regular channel group */
	ADC_init.DiscontinuousConvMode = DISABLE;								/*Specifies whether the conversion is performed in Discontinuous or not 
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
	// HAL_ADC_Start starts ADC conversions when the polling method is used
	HAL_ADC_Start(&ADC1_Handle);
}
	
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
