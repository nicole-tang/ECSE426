/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "gpio.h"


#define SAMPLINGCOUNTER 10
#define PROCESSINGCOUNTER 200
#define DISPLAYINGCOUNTER 1
#define ALARMCOUNTER 100
#define TEMPERATURETHRESHOLD 60


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;
volatile int systick_flag;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void initialize_ADC(void);
void initialize_GPIO_segments(void);
void initialize_GPIO_digits(void);
void initialize_GPIO_dp(void);
void initialize_GPIO_alarms(void);
float function_ADC(void);
float tempConversion(float);
void alarm_overheating(void);
void led_display(float,int);
int FIR_C(float* , float*, float*, int, int);
void led_number(int,int);
void led_unit(char);
float celsius_to_farenheit (float);
void initialize_GPIO_button(void);


int main(void)
{
	// initialize variables
	float adc_value,temperature;

	// initialize counters for sampling, processing, and displaying
	int sampling_count=0;
	int processing_count=0;
	int digit_count=0;
	int time_count=0;
	int alarm_count=0;
	// coefficient array for FIR filter
	float coeff[]={0.1,0.1,0.1,0.1,0.1};
	int filterArrayCounter=0;
	float inputArray[15];
	float filteredVoltage[15];
	
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

	/* Configure the system clock */
  SystemClock_Config();

 
	/*Initialize ADC*/
	initialize_ADC();
	initialize_GPIO_segments();
	initialize_GPIO_digits();
	initialize_GPIO_dp();	
	initialize_GPIO_alarms();	
	initialize_GPIO_button();
	
	
	
/*	// toggle the display
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	// the number to display
	led_unit('f');
*/


/* Infinite loop */
 while (1)
  {
		//run only at interrupts
		if(systick_flag==1){
			//set interrupt back to zero
			systick_flag=0;
			
			int button_pressed=0;
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)==GPIO_PIN_SET){
				button_pressed++;
			}
			if(button_pressed%2==1){
			// sample only when sampling count hits the number of sampling counter (10) 
			if(sampling_count++ >= SAMPLINGCOUNTER){
				//reset counter
				sampling_count=0;
				adc_value = function_ADC();
				printf("the adc_value is: %f \n", adc_value);
				
				if(filterArrayCounter<=15){					
					//store the adc_value at the array
					inputArray[filterArrayCounter]=adc_value;
					filterArrayCounter++;
				}else{
					filterArrayCounter=0;
				}

			}
			// process only when processing counter hits the number of processing counter (200)
			if(processing_count++ >=PROCESSINGCOUNTER){
				processing_count=0;
			
				//filter the sampled voltage
				FIR_C(inputArray, filteredVoltage, coeff,15,4);		
				for (int i =0;i<15;i++){
				printf("the filteredVoltage is: %f \n", filteredVoltage[i]);
				}
	
				//put print F and call farenheit
//				temperature=celsius_to_farenheit(tempConversion(*filteredVoltage));
				temperature=celsius_to_farenheit(tempConversion(adc_value));

				printf("the temperature (F) is: %f \n", temperature);
			}
			// display only when time counter hits the number of displaying counter (2)
			if(time_count++ >= DISPLAYINGCOUNTER){
				//reset time_count
				time_count=0;
				//if the increments of digit goes beyond 4, reset it
				if(digit_count++>=4){
					digit_count=0;
				}
				// if temperature is within threshold, display
				if(temperature >= TEMPERATURETHRESHOLD){
					// increment alarm count every CC but only execute the alarm if overheated every ALARMCOUNTER
					if(alarm_count++ >=ALARMCOUNTER){
						//reset alarm counter
						alarm_count=0;
						alarm_overheating();
					}
					//display temperature
					led_display(temperature, digit_count);
				}
				else{
					led_display(temperature, digit_count);
				}
			}
			}
			else{
			// sample only when sampling count hits the number of sampling counter (10) 
			if(sampling_count++ >= SAMPLINGCOUNTER){
				//reset counter
				sampling_count=0;
				adc_value = function_ADC();
				printf("the adc_value is: %f \n", adc_value);
				
				if(filterArrayCounter<=15){					
					//store the adc_value at the array
					inputArray[filterArrayCounter]=adc_value;
					filterArrayCounter++;
				}else{
					filterArrayCounter=0;
				}

			}
			// process only when processing counter hits the number of processing counter (200)
			if(processing_count++ >=PROCESSINGCOUNTER){
				processing_count=0;
			
				//filter the sampled voltage
				FIR_C(inputArray, filteredVoltage, coeff,15,4);		
				for (int i =0;i<15;i++){
				printf("the filteredVoltage is: %f \n", filteredVoltage[i]);
				}
				

//							temperature=tempConversion(*filteredVoltage);
				temperature=tempConversion(adc_value);	
				
				printf("the temperature (C) is: %f \n", temperature);
			}
			// display only when time counter hits the number of displaying counter (2)
			if(time_count++ >= DISPLAYINGCOUNTER){
				//reset time_count
				time_count=0;
				//if the increments of digit goes beyond 4, reset it
				if(digit_count++>=4){
					digit_count=0;
				}
				// if temperature is within threshold, display
				if(temperature >= TEMPERATURETHRESHOLD){
					// increment alarm count every CC but only execute the alarm if overheated every ALARMCOUNTER
					if(alarm_count++ >=ALARMCOUNTER){
						//reset alarm counter
						alarm_count=0;
						alarm_overheating();
					}
					//display temperature
					led_display(temperature, digit_count);
				}
				else{
					led_display(temperature, digit_count);
				}
			}
			
		}
  }
	
}
}

void alarm_overheating (void){
	__HAL_RCC_GPIOD_CLK_ENABLE();

	// orange LED
	for(int i=0; i<500000; i++){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); 
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); 		
	}  
	// red LED
	for(int i=0; i<500000; i++){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); 
	}
	// blue LED
	for(int i=0; i<500000; i++){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); 
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET); 
	}
	// green LED
	for(int i=0; i<10000; i++){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET); 		
	}
	
	
/*	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); 
*/
printf("too hot \n\n");
}


//converting celsius to farenheit
float celsius_to_farenheit(float celsius){
	
	printf("result from %f celsius_to_farenheit is:%f",celsius,((celsius*9/5)+32));
	return ((celsius*9/5)+32);
}





//convert when there is change from 0 to 1 and 1 to 0

//get current state and next state 
//if current state is not equal to next state then convert






int FIR_C(float* inputArray, float* outputArray, float* coeff, int length, int order){
	//for all sample in the inputArray
	for(int n=0; n<length-order; n++){
		//temp variable to store the accumulative sum of the filter
		float sum = 0;
		//iterate for the number of existing coefficients
		for(int b=0; b<=order; b++){
			//multiply the content of pointer coeff to content of point input array
			sum += inputArray[n+b] * coeff[b];
		}		
		//store the result
		outputArray[n] = sum;
	}
	return 0;
}
// Initialize ADC
void initialize_ADC(void){
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	ADC_InitTypeDef ADC_init;
	ADC_ChannelConfTypeDef channelConfig;
	
	/*First struct ADC_InitTypeDef*/
	ADC_init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;     									/*Select the frequency of the clock to the ADC*/
	ADC_init.Resolution = ADC_RESOLUTION_12B;               /*Choose resolution to be 12 bits, slower but more precision*/
	ADC_init.DataAlign =  ADC_DATAALIGN_RIGHT;              /*Data alignment is right */
	ADC_init.ScanConvMode = DISABLE;                        /*One channel mode*/
	ADC_init.EOCSelection = ADC_EOC_SEQ_CONV;               /*Perform ADC conversions without having to read all conversion data*/ 
	ADC_init.ContinuousConvMode = DISABLE;
	ADC_init.DMAContinuousRequests = DISABLE;               /*DMA request is not performed*/
	ADC_init.NbrOfConversion = 1;                           /*number of ADC conversions that will be done using sequencer for regular channel group */
	ADC_init.DiscontinuousConvMode = ENABLE;								
	//ADC_init.NbrOfDiscConversion - do not need this
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


// HAL_ADC_Start starts ADC conversions when the polling method is used

float function_ADC(void){
	float voltage = 0.0;
	HAL_ADC_Start(&ADC1_Handle);
	if((HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK)){
		voltage = HAL_ADC_GetValue(&ADC1_Handle);
	}
	HAL_ADC_Stop(&ADC1_Handle);
	return (voltage*3.0)/4096.0; // resolution is in 12 bits (4096 = 2^12) with Vref = 3V
}
	

// Conversion from voltage to celsius
float tempConversion(float voltage){
	float V_25 = 0.76;
	float avg_slope = 2.5/1000;
	return ((voltage-V_25)/avg_slope)+25;
}


// Initialize GPIO (General-purpose input/output)
void initialize_GPIO_segments(void){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOE,&GPIO_init);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10,GPIO_PIN_RESET);
}

void initialize_GPIO_digits(void){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOD,&GPIO_init);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_RESET);
}

void initialize_GPIO_dp(void){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_1;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB,&GPIO_init);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
}

void initialize_GPIO_alarms(void){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_PULLUP;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOD,&GPIO_init);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_PIN_RESET);
}

void initialize_GPIO_button(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_0;
	GPIO_init.Mode = GPIO_MODE_INPUT;
	GPIO_init.Pull = GPIO_PULLDOWN;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_init);
	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

/*
	7 segments display to STM32F4XX 
	PIN E4 = A 
	PIN E5 = B
	PIN E6 = C
	PIN E7 = D
	PIN E8 = E
	PIN E9 = F     
	PIN E10 = G

*/
void led_number(int number, int dp){
	switch(number){
		
		//Display number 0
		case 0:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET); //G
			printf("0 should be displayed \n");
			break;
		
		//Display number 1
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET); //G
			printf("1 should be displayed \n");
			break;		
		//Display number 2
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET); //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("2 should be displayed \n");
			break;		
		//Display number 3
		case 3:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A;
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("3 should be displayed \n");
			break;		
		//Display number 4
		case 4:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("4 should be displayed \n");
			break;		
		//Display number 5
		case 5:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E 
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("5 should be displayed \n");
			break;		
		//Display number 6
		case 6:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("6 should be displayed \n");
			break;		
		//Display number 7
		case 7:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET); //G
			printf("7 should be displayed \n");
			break;	
		//Display number 8
		case 8:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("8 should be displayed \n");
			break;
		
		//Display number 9
		case 9:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);   //G
			printf("9 should be displayed \n");
			break;

		default:
		break;
	}
	
	//set decimal point
	if(dp ==1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);		
	} 
	//reset decimal point
	else if(dp==0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
	} 
}
void led_unit(char c_or_f){
	switch(c_or_f){	
	//Display character c
		case 'c':
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);			// B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);				// D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);				// E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);		// G
			printf("C should be displayed \n");
			break;
			
		case 'f':
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);			// B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);			// D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);				// E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);		// G
			printf("F should be displayed \n");
			break;
	}
}
void led_display(float temperature,int digit)
{
	switch(digit){
		case 1:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
			
			// the number to display
			led_number((((int)temperature%100)-((int)temperature%10))/10,0);
			break;
		
		case 2:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			
			// the number to display and lower dot as decimal point
			led_number(((int)temperature%10),1);
			break;

		case 3:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			
			// the number to display
			led_number((((int)(temperature*10))%1000)-(((int)temperature%100)*10), 0);
			break;
					
		case 4:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
			
			GPIO_PinState nextState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

			int currentState = 0;
			if(currentState != nextState){
				//unit to display
				led_unit('f');
			} else{
				//unit to display
				led_unit('c');
			}
			

			break;
		
		default:
			break;
	}
}



/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	/* might need to change the number to 100? -> 10 ms instead of 1 ms?*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
