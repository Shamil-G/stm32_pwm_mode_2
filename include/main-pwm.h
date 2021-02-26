/*
 * pwm-2.h
 *
 *  Created on: Feb 21, 2021
 *      Author: Shamil Gusseynov
 */

// PP_MODE - Push Pull Mode - выравнивание ШИМ по середине с одинаковой длиной
// COMPL_MODE - если один сигнал уменьшается то другой увеличивается за его счет

#define STM32F750

#include "dev-include\stm32h743-pwm.h"
#include "dev-include\stm32f401-pwm.h"
#include "dev-include\stm32f407vet-pwm.h"

//  Разгон до 1/INIT_PART
#define INIT_PART			9
#define PWM_VALUE           1
#define PWM_PSC				0

#ifndef INC_PWM_2_H_

#define INC_PWM_2_H_

void start_pwm_2(void);
void PWM_init(void);
void soft_start(void);
void pwm_test(void);

// Код ниже не работает ...

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
