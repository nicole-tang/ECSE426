#include "lis3dsh.h"
#include "stm32f4xx_hal.h"
#include "accelerometer.h"
#include "gpio.h"
#include "cmsis_os.h"  
#include "timer.h"
#include "keypad.h"
#include "stdlib.h"

// Variables
float acc[3] = {0, 0, 0};
int pitch_output = 0;
int roll_output = 0; 
int pitch_input = 0;
int roll_input = 0;
int roll_degree_difference = 0;
int pitch_degree_difference = 0;


LIS3DSH_InitTypeDef LIS3DSH_InitType;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_DRY;


// Thread declaration
void Thread_acceleration(void const *argument);
osThreadId tid_Thread_acceleration; // Thread ID
osThreadDef(Thread_acceleration, osPriorityAboveNormal, 1, 800); // Thread name, priority, instances, stack size


// Start accelerometer thread
int start_Thread_acceleration(void)
{
	tid_Thread_acceleration = osThreadCreate(osThread(Thread_acceleration), NULL);
	if(!tid_Thread_acceleration) return (-1);
	return 0;
}


void Thread_acceleration(void const *argument)
{
	while(1)
	{
		osSignalWait(0x2, osWaitForever); // Wait for accelerometer signal

		reading_accel_values(acc);
		calibration_accel(acc);
		
		pitch_input = interpret_key();
		pitch_output = interpret_key();

		pitch_output = pitch_tilt_angle(acc);
		roll_output = roll_tilt_angle(acc);
		
		roll_degree_difference = abs(roll_input - roll_output);
		pitch_degree_difference = abs(pitch_input - pitch_output);
		
		/*
		TIM_CHANNEL_1 For green LED
		TIM_CHANNEL_2 For orange LED
		TIM_CHANNEL_3 For red LED
		TIM_CHANNEL_4 For blue LED
		*/
		change_pulse(roll_degree_difference, TIM_CHANNEL_1);
		change_pulse(pitch_degree_difference, TIM_CHANNEL_2);
		change_pulse(roll_degree_difference, TIM_CHANNEL_3);
		change_pulse(pitch_degree_difference, TIM_CHANNEL_4);
		
		// If degree different is less than 5, turn off the led
		if(pitch_degree_difference<=5)
		{
			turn_off_led(TIM_CHANNEL_2);
			turn_off_led(TIM_CHANNEL_4);
		}
		if(roll_degree_difference<=5)
		{
			turn_off_led(TIM_CHANNEL_1);
			turn_off_led(TIM_CHANNEL_3);
		}
	}
}


// Initialize accelerometer
void initialize_accel(void)
{	
	// Enabling the clock for GPIO port E
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	/* LIS3DSH struct to configure */
	LIS3DSH_InitType.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; // Output data 25 times per second (frequency of 25Hz)
	LIS3DSH_InitType.Axes_Enable = LIS3DSH_XYZ_ENABLE; // Enable x, y, and z axes
	LIS3DSH_InitType.Continous_Update = LIS3DSH_ContinousUpdate_Disabled; 
	LIS3DSH_InitType.AA_Filter_BW = LIS3DSH_AA_BW_50; 
	LIS3DSH_InitType.Full_Scale = LIS3DSH_FULLSCALE_2; // Full scale of +/- 2g by default
	LIS3DSH_InitType.Self_Test = LIS3DSH_SELFTEST_NORMAL; // Normal mode by default
	
	LIS3DSH_Init(&LIS3DSH_InitType);
	
	/* LIS3DSH Data ready interrupt struct */
	LIS3DSH_DRY.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED; // Enable DRY interrupt
  LIS3DSH_DRY.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL; // Interrupt Signal Active High (requirements)
  LIS3DSH_DRY.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED; // Interrupt type to be pulsed (requirements)
	
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_DRY);
	
	/* Initialize the value for GPIO port E pin 0 */
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Mode = GPIO_MODE_IT_FALLING; // External Interrupt Mode with Falling edge trigger detection
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOE, &GPIO_Init);

	/* Enable INT1 line to read interrupt signal */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn); // Enable IRQ for the EXTI Line 0 (LSI3DSH generates INT1 interrupt on GPIOE.0)
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 1); // Set priority for EXTI0_IRQ

}


// Read values from accelerometer
void reading_accel_values(float *acc)
{
	LIS3DSH_ReadACC(acc);
//	printf("ax: %f, ay: %f , az: %f \n",acc[0],acc[1],acc[2]);
}


// Function to return the tilt angle
float pitch_tilt_angle(float *acc)
{
	float pitch;
	pitch = 90 + RAD_TO_DEG(atan2f(acc[1], sqrtf(acc[0] * acc[0] + acc[2] * acc[2])));
	return pitch;
}


// Function to return the tilt angle
float roll_tilt_angle(float *acc)
{
	float roll;
	roll = 90 + RAD_TO_DEG(atan2f(acc[0], sqrtf(acc[1] * acc[1] + acc[2] * acc[2])));
	return roll;
}


void calibration_accel(float *acc)
{
	// matrix found using Doc 15, Least Square Method
	const float calib[4][3] = {
		{0.0010,-0.0000,0.0000},
   {0.0000,0.0010,0.0000},
   {-0.0000,0.0000,0.0010},
   {-0.0119,0.0010,-0.0209}
	};
	
	float new_x, new_y, new_z;
	
	new_x = calib[0][0] * (acc[0]) + calib[1][0] * (acc[1]) + calib[2][0] * (acc[2]) + calib[3][0];
	new_y = calib[0][1] * (acc[0]) + calib[1][1] * (acc[1]) + calib[2][1] * (acc[2]) + calib[3][1];
	new_z = calib[0][2] * (acc[0]) + calib[1][2] * (acc[1]) + calib[2][2] * (acc[2]) + calib[3][2];
	
	acc[0] = new_x;
	acc[1] = new_y;
	acc[2] = new_z;
}
