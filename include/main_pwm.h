/*
 * pwm-2.h
 *
 *  Created on: Feb 21, 2021
 *      Author: Shamil Gusseynov
 */

// PP_MODE - Push Pull Mode - выравнивание ШИМ по середине с одинаковой длиной
// COMPL_MODE - если один сигнал уменьшается то другой увеличивается за его счет

#define STM32F750

#include "stm32h743_pwm.h"

//  Разгон до 1/INIT_PART
#define INIT_PART			9
#define PWM_VALUE           1
#define PWM_PSC				0



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

#endif

#ifdef STM32F401CCU
#define MEANDR_TIMER_TICKS  839
#define __TIMER TIM4
#define MEANDR_TIMER_TICKS  839
#define FIRST_COUNTER  TIM4->CCR1
#define SECOND_COUNTER TIM4->CCR2
#define DEADTIME_TICKS      40

#define TIM2_CH1_PA         8
#define TIM2_CH2_PA         9
#define LED1_G_PC           9
#define LED2_B_PC           8
#endif


#ifndef INC_PWM_2_H_

#define INC_PWM_2_H_

void start_pwm_2(void);
void PWM_init(void);
void soft_start(void);
void pwm_test(void);

// Код ниже не работает ...

#ifdef STM32F401CCU
void GPIOENABLE(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	GPIOB->MODER |= ~GPIO_MODER_MODE6_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE7;
	GPIOB->MODER |= ~GPIO_MODER_MODE7_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE8;
	GPIOB->MODER |= ~GPIO_MODER_MODE8_1;
}
#endif


#ifdef STM32F407VET
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


#ifdef __TIM1
void GPIOENABLE(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;
	RCC->APB2ENR |=RCC_APB2ENR_TIM1EN;
	GPIOE->MODER &= ~GPIO_MODER_MODER9;
	GPIOE->MODER |= GPIO_MODER_MODER9_1;
	GPIOE->MODER &= ~GPIO_MODER_MODER11;
	GPIOE->MODER |= ~GPIO_MODER_MODER11_1;
	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR9;
}
#endif

#endif /* INC_PWM_2_H_ */
