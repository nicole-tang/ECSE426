#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "gpio.h"

//initialize GPIO pins for keypad column (3 columns)
void init_keypad_column(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init1;
	GPIO_init1.Pin = GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10; //pins 8, 9, 10 of port D correspond to rows
	GPIO_init1.Mode = GPIO_MODE_INPUT; //columns configure initially as input
	GPIO_init1.Pull = GPIO_PULLUP; //pull up mode enabled
	GPIO_init1.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOD,&GPIO_init1); 
}


//initialize GPIO pins fo keypad row (4 rows)
void init_keypad_row(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	GPIO_InitTypeDef GPIO_init2;
	GPIO_init2.Pin = GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15; //pins 12, 13, 14, 15 correspond to columns
	GPIO_init2.Mode = GPIO_MODE_OUTPUT_PP ; //rows configured initially as output
	GPIO_init2.Pull = GPIO_PULLDOWN;
	GPIO_init2.Speed = GPIO_SPEED_HIGH; 
	
	HAL_GPIO_Init(GPIOE,&GPIO_init2); 
}
