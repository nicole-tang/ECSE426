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
#define TEMPERATURETHRESHOLD 30


/* Private variables ---------------------------------------------------------*/

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
void led_number(int);
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
	int displaying_count=0;
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
 
	/*Initialize*/
	initialize_ADC();
	initialize_GPIO_segments();
	initialize_GPIO_digits();
	initialize_GPIO_dp();	
	initialize_GPIO_alarms();	
	initialize_GPIO_button();


/* Infinite loop */
 while (1)
  {
		// read button input
		GPIO_PinState currentState= GPIO_PIN_RESET;
		GPIO_PinState nextState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		
		//execute the code only at interrupts
		if(systick_flag==1){
			//set interrupt back to zero
			systick_flag=0;

			// sample only when sampling count hits the number of sampling counter (10ms) 
			if(sampling_count++ >= SAMPLINGCOUNTER){
				//reset counter
				sampling_count=0;
				//fetch ADC value
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
			
			// process only when processing counter hits the number of processing counter (200ms)
			if(processing_count++ >=PROCESSINGCOUNTER){
				processing_count=0;
		
				//filter the sampled voltage
				FIR_C(inputArray, filteredVoltage, coeff,15,4);		
				for (int i =0;i<15;i++){
				printf("the filteredVoltage is: %f \n", filteredVoltage[i]);
				}
	
				//set temperature
//			temperature=tempConversion(*filteredVoltage);
				temperature=tempConversion(adc_value);

				printf("the temperature (F) is: %f \n", temperature);
			}
			// display only when time counter hits the number of displaying counter (1ms)
			if(displaying_count++ >= DISPLAYINGCOUNTER){
				//reset displaying_count
				displaying_count=0;
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
				}else if (currentState!=nextState){
					currentState=nextState;
					// display temperature in farenheit
					led_display(celsius_to_farenheit(temperature), digit_count);
				}
				//display temperature in celsius
				else{
					led_display(temperature, digit_count);
				}
			}
			}
}
}

//converting celsius to farenheit
float celsius_to_farenheit(float celsius){
	
	printf("result from %f celsius_to_farenheit is:%f",celsius,((celsius*9/5)+32));
	return ((celsius*9/5)+32);
}



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


// Conversion from voltage to celsius
float tempConversion(float voltage){
	float V_25 = 0.76;
	float avg_slope = 2.5/1000;
	return ((voltage-V_25)/avg_slope)+25;
}

/** System Clock Configuration*/
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

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq());

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

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
