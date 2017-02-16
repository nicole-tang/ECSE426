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


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void initialize_ADC(void);
void initialize_GPIO(void);
//float function_ADC(void);
//float tempConversion(float);



int main(void)
{
	float V_25 = 0.76, avg_slope = 2.5/1000;
	float temp, temperature;
	uint32_t voltage;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

	/* Configure the system clock */
  SystemClock_Config();

 
	/*Initialize ADC*/
	initialize_ADC();
	initialize_GPIO();
	

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_ADC_Start(&ADC1_Handle);
		if((HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK))
		voltage = HAL_ADC_GetValue(&ADC1_Handle);
		HAL_ADC_Stop(&ADC1_Handle);
		printf("the voltage is %d\n",voltage);
		temp = voltage * (3.0/4096);
		printf("temp %f\n", temp);
		temperature = ((temp-V_25)/avg_slope)+25;
		printf("the temperature is %f\n", temperature);
		
		/*
		adc_value = function_ADC();
		printf("the adc_value is %f\n", adc_value);		
		temp = tempConversion(adc_value);
		printf("the temperature is %f\n", temp);
		*/
		
		
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

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
/*
float function_ADC(void){
	float voltage = 0.0;
	HAL_ADC_Start(&ADC1_Handle);
	printf("\nHAL_ADC_PollForConversion(&ADC1_Handle, 10)==%x",(int)HAL_ADC_PollForConversion(&ADC1_Handle, 10000));
	if((HAL_ADC_PollForConversion(&ADC1_Handle, 10000) == HAL_OK)){
		voltage = HAL_ADC_GetValue(&ADC1_Handle);
		printf("the voltage is %f\n",voltage);
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
*/

// Initialize GPIO (General-purpose input/output)
void initialize_GPIO(void){
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_NOPULL;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
}

/*
	7 segments display to STM32F4XX 
	PIN C1 = A 
	PIN C2 = B
	PIN C3 = C
	PIN C4 = D
	PIN C5 = E
	PIN C6 = E
	PIN C7 = G

*/
void led_number(int number, int dot){
	switch(number){
		
		//Display number 0
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //G
		
		//Display number 1
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //G
		
		//Display number 2
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
		
		//Display number 3
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
			
		//Display number 4
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
			
		//Display number 5
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET); //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E 
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
			
		//Display number 6
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET); //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
			
		//Display number 7
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //G
			
		//Display number 8
		case 8:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);   //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);   //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G
		
		//Display number 9
		case 9:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);   //B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);   //C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); //D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); //E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);   //F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);   //G

		//Display 'C'
		case 10:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);			// B
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);				// D
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);				// E
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);			// G
			break;
					
		default:
		break;
	}
	
	//Set lower dot
	if(dot ==1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);		//lower dot
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_RESET);	//upper dot
	} 
	//Set upper dot
	else if (dot ==2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);		//lower dot
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_SET);			//upper dot
	}
	//No dots
	else{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);	//lower dot
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_RESET);	//upper dot
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
			
			// the number to display
			led_number(10,0);
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