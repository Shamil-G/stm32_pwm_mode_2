/*
 * stm32h743_pwm.h
 *
 *  Created on: 26 февр. 2021 г.
 *      Author: sguss
 */

#ifndef INC_STM32H743_PWM_H_
#define INC_STM32H743_PWM_H_


#ifdef STM32F750

#include "stm32h7xx.h"
// TIM1, TIM8 -> APB2
// TIM2, TIM3, TIM4, TIM5 ... -> APB1
#define __TIMER TIM4
#define Alternate_Function 0b0010
#define MEANDR_TIMER_TICKS  1199
#define DEADTIME_TICKS      199
#define FIRST_COUNTER  TIM4->CCR1
#define SECOND_COUNTER TIM4->CCR2

#endif

#endif /* INC_STM32H743_PWM_H_ */
