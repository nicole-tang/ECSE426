#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "stm32f4xx_hal.h"
#include "gpio.h"

void set_keypad_column(void);
void set_keypad_row(void);
int get_column(void);
int get_row(void);
int get_key(void);

#endif