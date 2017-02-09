#include <stdio.h>
//#include "arm_math.h"
#include <math.h>

extern int FIR_asm(float* InputArray, float* OutputArray, float* coeff,int Length);

int subtraction(float* OriginalData, float* FilteredData, float* SubtractedArray, int Length){
	// for each element in the array, do the substraction and store in an array
	for(int i=0;i<Length;i++){
		SubtractedArray[i]=OriginalData[i]-FilteredData[i];
	}
	return 0;
}

float avg(float* SubtractedArray, int Length){
	float sum=0.;
	for(int i=0;i<Length;i++){
		// get the sum of all elements
		sum+=SubtractedArray[i];
	}
	// average = sum/number of elements
	return sum/Length;
}


float standard_deviation(float* SubtractedArray, int Length){
	float sum=0.;
	for (int i=0;i<Length;i++){
		//standard deviation = sqrt((sum of (element - mean))^2/number of elements)
		sum+=(SubtractedArray[i]-avg(SubtractedArray,Length))*(SubtractedArray[i]-avg(SubtractedArray,Length));
	}	
	return sqrt(sum/(Length-1));
}


int FIR_C(float* InputArray, float* OutputArray,float* coeff, int Length, int Order){
	//for all sample in the InputArray
	for(int n=0;n<Length-Order-1;n++){
		//temp variable to store the accumulative sum of the filter
		float sum=0;
		//iterate for the number of existing coefficients
		for(int b=0;b<=Order;b++){
			//multiply the content of pointer coeff to content of point input array
			sum+= InputArray[n+b] * coeff[b];
		}		
		//store the result
		OutputArray[n] = sum;
	}
	return 0;
}

int main()
{
	//define variables
	float coeff[]={0.1,0.15,0.5,0.15,0.1};
	float InputArray[]={2.,2.,3.,4.,5.,6.,7.,8.,7.,4.,5.};

	int Length=(sizeof InputArray / sizeof(float));
	int Order=(sizeof coeff/sizeof(float))-1;
	float OutputArray[Length-Order-1];
    
/*
	//call FIR_C
	int status_c = FIR_C(InputArray, OutputArray, coeff, Length, Order);
	printf("The status returned from FIR_C is: %d\n", status_c);
 
     for (int i=0;i<Length;i++){
        printf("The %i element of input is %f and the filtered output is %f.\n",i+1,InputArray[i],OutputArray[i]);
    }
*/	
	
	//initialize subtractedArray 
	float SubtractedArray[Length];
	subtraction(InputArray,OutputArray,SubtractedArray,Length);
  for (int i=0;i<Length;i++){
      printf("The %d element of the subtraction is %f.\n",i+1,SubtractedArray[i]);
  }
	//average
	float average = avg(SubtractedArray,Length);
	printf("The average is %f \n", average);
	//standard deviation
	float std = standard_deviation(SubtractedArray,Length);
	printf("The std is %f", std);
	
	
	
	// arm_sub_f32(original, filtered, arrays->cmsis_diff, length);
	// arm_mean_f32(arrays->cmsis_diff, length, &cmsis_avg);
	// arm_std_f32(arrays->cmsis_diff, length, &cmsis_std);
	
	
	//call FIR_asm
	FIR_asm(InputArray, OutputArray, coeff, Length);
  for (int i=0;i<Length;i++){
        printf("The %i element of input is %f and the filtered output is %f.\n",i+1,InputArray[i],OutputArray[i]);
  }
	return 0;
}
