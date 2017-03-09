/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "main.h"
#include "stdint.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void initialize_timer(void);
void change_pulse(int degree_difference, uint32_t Channel);

#endif /* __TIMER_H */
