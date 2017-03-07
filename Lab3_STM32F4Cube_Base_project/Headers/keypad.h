#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "gpio.h"

void set_keypad_column(void);
void set_keypad_row(void);
uint8_t get_column(void);
uint8_t get_row(void);
int get_key(void);

#endif