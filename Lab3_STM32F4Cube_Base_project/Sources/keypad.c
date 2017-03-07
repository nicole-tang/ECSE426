#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "keypad.h"
#include <stdio.h>

/* Private Variables */
const uint16_t col_pinmap[3] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10};
const uint16_t row_pinmap[4] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

const uint8_t keypad_map[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{11, 0, 12}
};

GPIO_InitTypeDef GPIO_init1;
GPIO_InitTypeDef GPIO_init2;

// Initialization of pins to allow column read
void set_keypad_column(void)
{
	// Initialize column
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_init1.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10; 
	GPIO_init1.Mode = GPIO_MODE_INPUT; // Configure column as input
	GPIO_init1.Pull = GPIO_PULLUP; // Pin value set to high
	GPIO_init1.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD,&GPIO_init1); 
	
	// Initialize row
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
	// Initialize column
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
uint8_t get_column(void)
{
	set_keypad_column();
	
	if(!HAL_GPIO_ReadPin(GPIOD, col_pinmap[0]))
	{
		return 0;
	}
	else if(!HAL_GPIO_ReadPin(GPIOD, col_pinmap[1]))
	{
		return 1;
	}
	else if(!HAL_GPIO_ReadPin(GPIOD, col_pinmap[2]))
	{
		return 2;
	}
	else return 8;
}



// Function to return the row pressed
uint8_t get_row(void)
{
	set_keypad_row();
	
	if(!HAL_GPIO_ReadPin(GPIOE, row_pinmap[0]))
	{
		return 0;
	}
	else if(!HAL_GPIO_ReadPin(GPIOE, row_pinmap[1]))
	{
		return 1;
	}
	else if(!HAL_GPIO_ReadPin(GPIOE, row_pinmap[2]))
	{
		return 3;
	}
	else if(!HAL_GPIO_ReadPin(GPIOE, row_pinmap[3]))
	{
		return 4;
	}
	else return 8;
}


int get_key(void)
{
	uint8_t key;
	uint8_t column, row;
	
	key = keypad_map[get_row()][get_column()];

	return key;
}
