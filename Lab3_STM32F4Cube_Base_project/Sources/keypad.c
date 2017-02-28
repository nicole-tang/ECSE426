#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "keypad.h"

void init_keypad_column(void)
{
	__HAL_RCC_GPIOD_CLCK_ENABLE(); //to change
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin
	GPIO_init.Mode
	GPIO_init.Pull
	GPIO_init.Speed 
	HAL_GPIO_Init(GPIOE,&GPIO_init); //to change

	
}

void init_keypad_row(void)
{
	__HAL_RCC_GPIOD_CLCK_ENABLE(); //to change
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin
	GPIO_init.Mode
	GPIO_init.Pull
	GPIO_init.Speed 
	HAL_GPIO_Init(GPIOE,&GPIO_init); //to change

}


