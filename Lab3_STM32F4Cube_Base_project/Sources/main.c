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

#define DISPLAYINGCOUNTER 1


int systick_flag;
int flag;

/* Private variables ---------------------------------------------------------*/
float* ax;
float* ay;
float* az;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void initialize_GPIO_segments(void);
void initialize_GPIO_digits(void);
void initialize_GPIO_led_lights(void);
void deinitialize_GPIO_button(void);
void initialize_accel(void);
void reading_accel_values(float *acc);
float pitch_tilt_angle(float *acc);
float roll_tilt_angle(float *acc);
void set_keypad_column(void);
void set_keypad_row(void);
int get_column(void);
int get_row(void);
int get_key(void);
int interpret_key(void);

void led_lights(char color);
void led_number(int number);
void led_unit(char degree);
void led_display(int number,int digit);

void initialize_timer(void);
void change_pulse(int degree_difference);



int main(void)
{	
	int displaying_count=0;
	int digit_count=0;
	int input_angle=0;
	int input_pitch=0;
	int input_roll=0;
	
	float acc[3] = {0,0,0}; // Empty array to store the acceleration values
	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	initialize_GPIO_segments();
	initialize_GPIO_digits();
	initialize_GPIO_led_lights();
	deinitialize_GPIO_button();
	initialize_accel();
	initialize_timer();

	
/*	input_pitch=interpret_key();
	printf("input_pitch is %d",input_pitch);
	input_roll=interpret_key();
	printf("input_roll is %d",input_roll);
*/	
	while (1){
		if(systick_flag==1){
			systick_flag=0;
		// display only when time counter hits the number of displaying counter (1ms)
			if(displaying_count++ >= DISPLAYINGCOUNTER){
				//reset displaying_count
				displaying_count=0;
				//if the increments of digit goes beyond 4, reset it
				if(digit_count++>=4){
					digit_count=0;
				}
				//display temperature
					led_display(input_angle, digit_count);
			}
		}
		if(flag == 1){
	
			//reset flag
			flag = 0;
			
			// reading the inputs
		
			
			
			//processing		
			reading_accel_values(acc);
			printf("at main, accel_values are %f,%f,%f\n",acc[0],acc[1],acc[2]);
			int pitch = pitch_tilt_angle(acc);
			int roll = roll_tilt_angle(acc);
			
//			printf("pitch is %d\n",pitch);
//			printf("roll is %d\n",roll);
			
			int pitch_degree_difference=input_angle-pitch;
			int roll_degree_difference=input_angle-roll;
//			printf("pitch_degree_difference is %d\n",pitch_degree_difference);
//			printf("roll_degree_difference is %d\n",roll_degree_difference);
			
			// for LED light intensity display
			
			change_pulse(pitch_degree_difference);
			led_lights('o');
			led_lights('r');
			led_lights('b');
			led_lights('g');
		}
		
	}
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
