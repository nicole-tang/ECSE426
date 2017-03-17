#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "keypad.h"
#include <stdio.h>
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "display.h"
#include "lis3dsh.h"

const int keypad_map[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{11, 0, 12}
};

// Variables
int enter_counter = 0;
int angle = 0;
int key_is_pressed = 0;

GPIO_InitTypeDef GPIO_init1;
GPIO_InitTypeDef GPIO_init2;

// Initialization of pins to allow column read
void set_keypad_column(void)
{
	// Initialize the columns
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_init1.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10; 
	GPIO_init1.Mode = GPIO_MODE_INPUT; // Configure column as input
	GPIO_init1.Pull = GPIO_PULLUP; // Pin value set to high
	GPIO_init1.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD,&GPIO_init1); 
	 
	// Initialize the rows
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	GPIO_init2.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIO_init2.Mode = GPIO_MODE_OUTPUT_PP; // Configure row as output
	GPIO_init2.Pull = GPIO_PULLDOWN; // Pin value is set to low
	GPIO_init2.Speed = GPIO_SPEED_HIGH; 
	HAL_GPIO_Init(GPIOE,&GPIO_init2); 
}


// Initialization of pins to allow row read
void set_keypad_row(void)
{
	// Initialize the columns
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_init1.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10; 
	GPIO_init1.Mode = GPIO_MODE_OUTPUT_PP; // Column is now configured to output
	GPIO_init1.Pull = GPIO_PULLDOWN; // Pin value set to low
	GPIO_init1.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD,&GPIO_init1); 
	
	// Initialize the rows
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	GPIO_init2.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIO_init2.Mode = GPIO_MODE_INPUT; // Row is now configured to input
	GPIO_init2.Pull = GPIO_PULLUP; // Pin value set to high
	GPIO_init2.Speed = GPIO_SPEED_HIGH; 
	HAL_GPIO_Init(GPIOE,&GPIO_init2); 
}


// Function to return the column pressed
int get_column(void)
{
	// The columns are inputs and are set to high
	set_keypad_column();
	
	// Return the value of column pressed (pin of column pressed is going to be low)
	if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8) == GPIO_PIN_RESET)
	{
		return 0;
	}
	else if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_RESET)
	{
		return 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_RESET)
	{
		return 2;
	}
	else return -1;
}


// Function to return the row pressed
int get_row(void)
{
	// The rows are inputs and are set to high
	set_keypad_row();
	
	// Return the value of row pressed (pin of row pressed is going to be low)
	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == GPIO_PIN_RESET)
	{
		return 0;
	}
	else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == GPIO_PIN_RESET)
	{
		return 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == GPIO_PIN_RESET)
	{
		return 2;
	}
	else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_RESET)
	{
		return 3;
	}
	else return -1;
}


// Function to return the key pressed
int get_key(void)
{
	int key;
	int column = get_column();
	int row = get_row();
	
	if( (row == -1) || (column == -1)){ // If key is not pressed
		key = -1;
	}else{
		key = keypad_map[row][column];
	}
	return key;
}


//Function to reset input angle operation
int reset_key(void)
{
	int key = get_key();
	if(key == 11)
	{
		int counter = 0;
		int reset_counter = 0;
		
		while(counter < 100000)
		{
			if(get_key() == 11)
			{
				reset_counter++;
			}
			counter++;
		}
		
		if(reset_counter > 50000)
		{
			return 1;
		} 
		else 
		{
			return 0;
		}
	} 
	else 
	{
		return 0;
	}
}


// Function to relate input digits to input angle
int interpret_key(void)
{
	int key;
	int unpressed_counter = 0;
	int counter = 0;
	int break_off = 0;
	
	while(1)
	{		
		if(key_is_pressed == 1){
		key = get_key();
				if(key == -1)
				{
					unpressed_counter++; // Counter increments when nothing is pressed
				}
				else if(counter < 3 && key < 10 && unpressed_counter > 100) // If a digit between 0 and 9 is pressed 
				{ 
					angle = angle * 10;
					angle = angle + key;
					counter++;
					unpressed_counter = 0;
					printf("%d\n", angle);
				}
				else if(counter > 0 && key == 11 && unpressed_counter > 100) // If '*' is pressed
				{ 
					counter--;
					unpressed_counter = 0;
					angle = angle / 10;
					printf("%d\n",angle);
				}
				else if(key == 12 && unpressed_counter > 100 && counter > 0) // If '#' is pressed
				{	
					if(angle > 180)
						{
							printf("\nPlease enter an angle between 0 and 180\n");
							angle = 0;
							counter = 0;
						}		
						else 
						{
							key_is_pressed = 0;
						}
				}
				else if(reset_key()==1) // If '* is pressed for a long time, reset the entire system
				{
					NVIC_SystemReset();
				}
			}
		}
	return angle;
}


// Function to check if key is pressed
int is_key_pressed(void)
{
	int key;
	int unpressed_counter = 0;
		while(key_is_pressed == 0)
		{
			key = get_key();
			if(key == -1)
			{
				unpressed_counter++; // Counter increments when nothing is pressed
			}
			else if((key < 10) && (unpressed_counter > 100))
			{
				key_is_pressed = 1;
			}
		}
		return key_is_pressed;
}

