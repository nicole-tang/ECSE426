/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

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
	
	
printf("too hot \n\n");
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
void led_number(int number){
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);  //G
			printf("9 should be displayed \n");
			break;

		default:
		break;
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
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);			// G
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
			// don't need decimal point here
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			// the number to display
			led_number((((int)temperature%100)-((int)temperature%10))/10);
		
			break;
		
		case 2:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			// need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			
			// the number to display and lower dot as decimal point
			led_number((int)temperature%10);

			break;

		case 3:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			// don't need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);	
			// the number to display
			led_number((((int)(temperature*10))%1000)-(((int)temperature%100)*10));
	
		break;
					
		case 4:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
			// don't need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			GPIO_PinState nextState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

			// set to display C or F 
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

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PXX   ------> YYYYYY
		 
		 Continue your GPIO configuration information as abow.

*/

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
