#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "keypad.h"
#include <stdio.h>


const char keypad_map[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{'*', 0, '#'}
};


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
		return 1;
	}
	else if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9) == GPIO_PIN_RESET)
	{
		return 0;
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
		return 3;
	}
	else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == GPIO_PIN_RESET)
	{
		return 2;
	}
	else if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_RESET)
	{
		return 1;
	}
	else return -1;
}


// Function to return the key pressed
int get_key(void)
{
	int key;
	int column = get_column();
	int row = get_row();
	

	key = keypad_map[row][column];

	printf("the key is %d\n", key);
	return key;
}


int interpret_key(void)
{
	int key = get_key();
	int angle=0;
	
	while(1){
		if(key!='#'||key!='*'||key!=-1){
			angle=angle*10;
			angle=angle+key;
		}else if(key=='*'){
			if(angle>=10){
				angle=angle%10;
			}else{
				angle=0;
			}
		}else if(key=='#'){
			break;
		}
	}
	return angle;

}

