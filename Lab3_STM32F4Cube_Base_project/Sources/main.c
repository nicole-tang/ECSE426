/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "gpio.h"
#include "keypad.h"
#include "accelerometer.h"
#include "main.h"
#include "stdlib.h"
#include "timer.h"

#define DISPLAYINGCOUNTER 1


int systick_flag;
int flag;

/* Variables -------------------------------------------------------------------*/
float* ax, ay, az;
int digit_count = 0;
int input_pitch = 0;
int input_roll = 0;
int pitch = 0, filtered_pitch = 0;
int roll = 0, filtered_roll = 0;
int pitch_degree_difference = 0;
int roll_degree_difference = 0;
float acc[3] = {0,0,0}; // Empty array to store the acceleration values

//to be configured
float coeff[5]={0.35,0.20,0.15,0.17,0.15};
float PitchInputArray[]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
float RollInputArray[]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
int Length = 40;
int Order = 4;
void store_result(float* InputArray, int Length, float newData);
float FIR_C(float* InputArray, float* coeff, int Length, int Order);



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);

void initialize_GPIO_led_lights(void);
void deinitialize_GPIO_button(void);

void initialize_accel(void);
void reading_accel_values(float *acc);
float pitch_tilt_angle(float *acc);
float roll_tilt_angle(float *acc);
void calibration_accel(float *acc);

void set_keypad_column(void);
void set_keypad_row(void);
int get_column(void);
int get_row(void);
int get_key(void);
int interpret_key(void);
int reset_key(void);

void initialize_timer(void);
void change_pulse(int degree_difference, uint32_t Channel);
void turn_off_led(uint32_t Channel);


int main(void)
{	

  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	initialize_GPIO_led_lights();
	deinitialize_GPIO_button();
	initialize_accel();
	initialize_timer();

	// prompt for input
	printf("Please enter an input angle for the pitch\n");
	input_pitch = interpret_key();	
	printf("\nThe input angle for the pitch is %d\n",input_pitch);
	
	printf("\nPlease enter an input angle for the roll\n");
	input_roll = interpret_key();	
	printf("\nThe input angle for the roll is %d\n",input_roll);
	

	while (1){
		// If '*' is pressed for a long time, reset the input angle operation

		if(reset_key() == 1)
		{
			printf("Please enter an input angle for the pitch\n");
			input_pitch = interpret_key();	
			printf("\nThe input angle for the pitch is %d\n",input_pitch);
			
			printf("\nPlease enter an input angle for the roll\n");
			input_roll = interpret_key();	
			printf("\nThe input angle for the roll is %d\n",input_roll);
		}
		// flag from data ready interrupt
		else if(flag == 1){
			//reset flag
			flag = 0;
			
			//processing		
			reading_accel_values(acc);
			calibration_accel(acc);
			
			pitch = pitch_tilt_angle(acc);
			roll = roll_tilt_angle(acc);

			//uncomment this to print unfiltered result
			printf("%d,%d\n",pitch,roll);

			store_result(PitchInputArray, Length, pitch);
			store_result(RollInputArray, Length, roll);

			filtered_pitch = FIR_C(PitchInputArray,coeff,Length,Order);
			filtered_roll = FIR_C(RollInputArray,coeff,Length,Order);			

			pitch_degree_difference=abs(input_pitch-filtered_pitch);
			roll_degree_difference=abs(input_roll-filtered_roll);
			
			/*
				TIM_CHANNEL_1 For green LED
				TIM_CHANNEL_2 For orange LED
				TIM_CHANNEL_3 For red LED
				TIM_CHANNEL_4 For blue LED
			*/
			change_pulse(roll_degree_difference,TIM_CHANNEL_1);
			change_pulse(pitch_degree_difference,TIM_CHANNEL_2);
			change_pulse(roll_degree_difference,TIM_CHANNEL_3);
			change_pulse(pitch_degree_difference,TIM_CHANNEL_4);		

			// if degree different is less than 5, turn off the led
				if(pitch_degree_difference<=5){
					turn_off_led(TIM_CHANNEL_2);
					turn_off_led(TIM_CHANNEL_4);
				}
				if(roll_degree_difference<=5){
					turn_off_led(TIM_CHANNEL_1);
					turn_off_led(TIM_CHANNEL_3);
				}
		}
	}
}

//Filter

float FIR_C(float* InputArray, float* coeff, int Length, int Order){
	float OutputArray[Length-Order-1];
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
	return OutputArray[0];
}


void store_result(float* InputArray, int Length, float newData){
//shift array
for (int i=Length-1;i>0;i--){
	InputArray[i-1]=InputArray[i];
}
// put the new data onto the last element of the array
	InputArray[Length-1]=newData;
} 


/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
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
void assert_failed(uint8_t* file, uint32_t line){
}
#endif
