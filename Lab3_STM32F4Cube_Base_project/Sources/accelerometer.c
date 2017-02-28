#include "lis3dsh.h"
#include "stm32f4xx_hal.h"

LIS3DSH_InitTypeDef LIS3DSH_InitType;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_DRY;


void initialize_accel(void)
{	
	/* LIS3DSH struct */
	LIS3DSH_InitType.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25; //Output data 25 times per second (frequency of 25Hz)
	LIS3DSH_InitType.Axes_Enable = LIS3DSH_XYZ_ENABLE; //Enable x, y, and z axes
	LIS3DSH_InitType.Continous_Update = LIS3DSH_ContinousUpdate_Enabled; //
	LIS3DSH_InitType.AA_Filter_BW = LIS3DSH_AA_BW_50; //
	LIS3DSH_InitType.Full_Scale = LIS3DSH_FULLSCALE_2; //Full scale of +/- 2g by default
	LIS3DSH_InitType.Self_Test = LIS3DSH_SELFTEST_NORMAL; //Normal mode by default
	
	LIS3DSH_Init(&LIS3DSH_InitType);
	
	/* LIS3DSH Data ready interrupt struct */
	LIS3DSH_DRY.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_DISABLED; //Enable DRY interrupt
  LIS3DSH_DRY.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL; // Interrupt Signal Active High (requirements)
  LIS3DSH_DRY.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED; // Interrupt type to be pulsed (requirements)
	
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_DRY);
}

//Read values from accelerometer
void reading_accel_values(float *ax, float *ay, float *az)
{
	float acc[3]; //empty array to store the acceleration values
	
	LIS3DSH_ReadACC(acc);
	*ax = acc[0];
	*ay = acc[1];
	*az = acc[2];
}

//Calibrate accelerator
void calibration_accel(float *ax, float *ay, float *az)
{
	
}



