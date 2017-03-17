#include "stm32f4xx_hal.h"
#include "math.h"
#include "cmsis_os.h"
#include "adc.h" 
#include "display.h"
#include "keypad.h"
#include "accelerometer.h"

#define HEAT_THRESHOLD 60
#define FLASH_INTERVAL 70

void Thread_display (void const *argument);                 // thread function
osThreadId tid_Thread_display;                              // thread id
osThreadDef(Thread_display, osPriorityNormal, 1, 0);


int start_Thread_display(void)
{
	printf("start_Thread_display\n");
	tid_Thread_display = osThreadCreate(osThread(Thread_display), NULL);
	if(!tid_Thread_display) return (-1);
	return (0);
}


void Thread_display(void const *argument)
{
	printf("Thread_display\n");
	int is_flashing, flash_counter, flash_on, flash_off, display_value;
	
	flash_counter = 0;
	is_flashing = 0;
	flash_on = FLASH_INTERVAL;
	flash_off = FLASH_INTERVAL * 2;
	int displaying_count=0;
	int digit_count=0;
	
	
	initialize_GPIO_dp();
	initialize_GPIO_digits();
	initialize_GPIO_segments();
	
	while(1)
	{
		if(filtered_temp > HEAT_THRESHOLD)
		{
			is_flashing = 1;
		} else {
			is_flashing = 0;
		}
		
		if(mode == 1){	//temperature mode 
			display_value = filtered_temp;
		}else if(mode == 2){	//accelerometer mode
			display_value = angle;
		}
		
		if(is_flashing == 1){

				//if the increments of digit goes beyond 4, reset it
				if(digit_count++>=4){
					digit_count=0;
				}
					osSignalWait(0x5,osWaitForever);
					led_display(display_value, digit_count,'o',mode);
			//	printf("%d should be displaying\n", display_value);

					//osDelay(50);
				//	turn_off_seven_segment();
				//	osDelay(50);
		}else{

				//if the increments of digit goes beyond 4, reset it
				if(digit_count++>=4){
					digit_count=0;
				}
							osSignalWait(0x4,osWaitForever);
					led_display(display_value, digit_count,'o',mode);
					//		printf("%d should be displaying\n", display_value);
		}
			
	}
}


/*initialization*/
// Initialize GPIO (General-purpose input/output)
void initialize_GPIO_segments(void){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_PULLDOWN;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOE,&GPIO_init);
}


void initialize_GPIO_digits(void){
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_PULLDOWN;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOD,&GPIO_init);
}


void initialize_GPIO_dp(void){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_init;
	GPIO_init.Pin = GPIO_PIN_1;
	GPIO_init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_init.Pull = GPIO_PULLDOWN;
	GPIO_init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOB,&GPIO_init);
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
//			printf("0 should be displayed \n");
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
//			printf("1 should be displayed \n");
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
//		printf("2 should be displayed \n");
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
//			printf("3 should be displayed \n");
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
//			printf("4 should be displayed \n");
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
//			printf("5 should be displayed \n");
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
//			printf("6 should be displayed \n");
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
//			printf("7 should be displayed \n");
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
//			printf("8 should be displayed \n");
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
//			printf("9 should be displayed \n");
			break;

		default:
		break;
	}
}


void led_unit(char c_f_o){
	switch(c_f_o){	
	//Display character c
		case 'c':
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);			// B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);				// D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);				// E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);		// G
//			printf("C should be displayed \n");
			break;
			
		case 'f':
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);			// B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);			// D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);				// E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);			// G
//			printf("F should be displayed \n");
			break;
		
		case 'o':
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);				// A
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);				// B
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);			// C
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);			// D
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);			// E
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);				// F
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);			// G
//			printf("o should be displayed \n");
			break;
	}
}


void led_display(int number,int digit,char unit,int mode){
	if(mode == 1){//temperature mode
	switch(digit){
		case 1:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);		
		// the number to display
			led_number(((number%100)-(number%10))/10);
		
			break;
		
		case 2:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		// need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);	
		// the number to display and lower dot as decimal point
			led_number(number%10);

			break;

		case 3:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);		
		// the number to display
			led_number((((number*10))%1000)-((number%100)*10));
	
		break;
					
		case 4:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);		
		// display the degree unit 
			led_unit(unit);
			break;
		
		default:
			break;
	}
}else{//acc mode
			switch(digit){
		case 1:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);	
		// the number to display
			led_number(((number%1000)-(number%100))/100);
		
			break;
		
		case 2:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);		
		// the number to display and lower dot as decimal point
			led_number(((number%100)-(number%10))/10);

			break;

		case 3:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);	
		// the number to display
			led_number(number%10);
	
		break;
					
		case 4:
			// toggle the display
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	// do not need decimal point here	
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);	
		// display the degree unit 
			led_unit(unit);
			break;
		
		default:
			break;
		}
	}
}

void turn_off_seven_segment(void){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
}



