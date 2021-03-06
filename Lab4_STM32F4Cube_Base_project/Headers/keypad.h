#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"


void set_keypad_column(void);
void set_keypad_row(void);
int get_column(void);
int get_row(void);
int get_key(void);
int interpret_key(void);
int reset_key(void);
void is_key_pressed(void);

//void Thread_keypad(void const *argument);
//int start_Thread_keypad(void);

extern int key_is_pressed;
extern int angle;

#endif

