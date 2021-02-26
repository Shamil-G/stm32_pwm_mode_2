/*
 * stm32F407VET.h
 *
 *  Created on: 26 февр. 2021 г.
 *      Author: sguss
 */

#ifndef INC_STM32F407VET_PWM_H_
#define INC_STM32F407VET_PWM_H_

#ifdef STM32F407VET
// TIM1, TIM8 -> APB2
// TIM2, TIM3, TIM4, TIM5 ... -> APB1
#define __TIMER TIM4
#define MEANDR_TIMER_TICKS  839
#define DEADTIME_TICKS      40
#define FIRST_COUNTER  TIM4->CCR1
#define SECOND_COUNTER TIM4->CCR2

#define TIM1_CH1N_PA        PA7
#define TIM1_CH1_CH1N       PA7 PE9
#define TIM1_CH1_PE         PE9
#define TIM1_CH2_PE         PE11

#define TIM2_CH1_PA			A5
#define TIM2_CH2_PB			B3

#define TIM4_PWM_CH1_PD			D12
#define TIM4_PWM_CH2_PD			D13
#define TIM4_OUT_CH3_PD			D14
#define LED1_PA           6
#define LED2_PA           7

void GPIOENABLE(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	GPIOD->MODER &= ~GPIO_MODER_MODER12; //will be pwm mode 1
	GPIOD->MODER |= GPIO_MODER_MODER12_1;
	GPIOD->MODER &= ~GPIO_MODER_MODER13; //will be pwm mode 2
	GPIOD->MODER |= ~GPIO_MODER_MODER13_1;
	GPIOD->MODER &= ~GPIO_MODER_MODER14;
	GPIOD->MODER |= ~GPIO_MODER_MODER14_1; //will be output
}

#endif

#endif /* INC_STM32F407VET_PWM_H_ */
