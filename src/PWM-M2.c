/*
 * pwm.c
 *
 *  Created on: 10 окт. 2020 г.
 *      Author: sguss
 */
//#define STM32F401CCU
#define STM32F750

#ifndef COMPL_MODE
#define PP_MODE
#endif

// Push Pull Mode
//CH1: PWM mode 2, CH2: PWM mode 1, preload enabled on all channels

#include "main.h"
#include "pwm-2.h"
void led1_on(void);
void led2_on(void);
void led1_off(void);
void led2_off(void);

void start_pwm_2(){
//*
//	GPIOENABLE(); -- Пока не работает 2
//	TIMx_CCMR1 - capture/compare mode register
#ifdef PP_MODE
    // Делитель частоты таймера
    __TIMER->PSC = PWM_PSC;

    // ARR - auto reload register
    __TIMER->ARR=MEANDR_TIMER_TICKS;

    // Счетчик регистра захвате/сравнения первого канала
    FIRST_COUNTER=(MEANDR_TIMER_TICKS/INIT_PART)-(DEADTIME_TICKS/2)-PWM_VALUE;
    // Счетчик регистра захвате/сравнения второго канала
    SECOND_COUNTER=(MEANDR_TIMER_TICKS*(INIT_PART-1)/INIT_PART)+(DEADTIME_TICKS/2)+PWM_VALUE;

    // Третий канал никогда не выключается, нужен для тактирования АЦП
    __TIMER->CCMR1=TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE | TIM_CCMR2_OC3PE;

    // Включаем PWM_mode=1 для 1 канала, output compare fast enable
    __TIMER->CCMR1|=TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1FE;
    // Включаем PWM_mode=2 для 2 канала
    __TIMER->CCMR1|=TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    // Третий порт: Toggle on match, в быстром режиме
    __TIMER->CCMR2 &=~TIM_CCMR2_OC3M;
    __TIMER->CCMR2 |= TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3FE;

    // CaptureCompareEnable Register: CaptureCompareEnable каналы 1 и 2 таймера и 3 каналы
    __TIMER->CCER=TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;
    // Делаем выходы таймера активными
    // BDTR - break and dead time register
    __TIMER->BDTR|=TIM_BDTR_MOE;

    // Задаем период меандра в тиках счетчика
    // CR1- Control Register, включаем автоматическую загрузку-перезагрузку таймера
    __TIMER->CR1=TIM_CR1_ARPE;
    // выравнивание по сигналов центру,
    // флаг прерывания при счете вверх и вниз: 5 и 6 биты = 11
    __TIMER->CR1|= TIM_CR1_CMS_1 | TIM_CR1_CMS_0;
    // Включаем счетчик, таймер начинает работать
    __TIMER->CR1|=TIM_CR1_CEN;
    // Обновляем счетчик: инициализация, выставление в начальное значение
    __TIMER->EGR=TIM_EGR_UG;

#endif

#ifdef COMPL_MODE
    //CH1: PWM mode with complementary output & deadtime
    __TIMER->CCMR1=TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    //
    __TIMER->CCER=TIM_CCER_CC1E | TIM_CCER_CC1NE;
    // Делаем выходы таймера активными
    __TIMER->BDTR=TIM_BDTR_MOE;
    __TIMER->BDTR|= DEADTIME_TICKS;
    // Делитель частоты таймера
    __TIMER->PSC = PWM_PSC;
    //
    __TIMER->CCR1=INIT_PWM_VALUE;
    __TIMER->CCR2=(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2 - INIT_PWM_VALUE;
    // Задаем период меандра в тиках счетчика
    __TIMER->ARR=MEANDR_TIMER_TICKS;
    // Включаем автоматическую загрузку-перезагрузку таймера
    __TIMER->CR1=TIM_CR1_ARPE;
    // Включаем счетчик, таймер начинает работать
    __TIMER->CR1|=TIM_CR1_CEN;
    // Обновляем счетчик: инициализация, выставление в начальное значение
    __TIMER->EGR=TIM_EGR_UG;
#endif
//*/
}
//*
#ifdef PP_MODE
// Второй канал, который в моде=1,  счетчик считает обычно по возрастанию
// Первый канал считает в обратную сторону

uint8_t pwm_down(uint16_t pwm_value){
	volatile uint8_t add_value, minus_value = 0;
	minus_value= (FIRST_COUNTER<pwm_value)?FIRST_COUNTER:pwm_value;
	add_value = (SECOND_COUNTER+pwm_value>=MEANDR_TIMER_TICKS)?MEANDR_TIMER_TICKS-SECOND_COUNTER:pwm_value;
	if (add_value>0 || minus_value>0){
//		led1_on();
		FIRST_COUNTER -= minus_value ;
		SECOND_COUNTER += add_value;
		return 1;
	}
//	led2_off();
	return 0;
};

uint8_t pwm_up(uint16_t pwm_value){
	volatile uint8_t add_value, minus_value = 0;
	add_value = (FIRST_COUNTER+pwm_value>=(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2)?(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2-FIRST_COUNTER:pwm_value;
	minus_value = (SECOND_COUNTER-pwm_value<=(MEANDR_TIMER_TICKS+DEADTIME_TICKS)/2)?SECOND_COUNTER-(MEANDR_TIMER_TICKS+DEADTIME_TICKS)/2:pwm_value;
	if (minus_value>0 || add_value>0){
//		led2_on();
		FIRST_COUNTER += add_value;
		SECOND_COUNTER -= minus_value;
		return 1;
	}
//	led1_off();
//	GPIOA->BSRR |= GPIO_BSRR_BR_7;
	return 0;
};

void soft_start(){
	volatile uint8_t add_value, minus_value = 0;
  init_adc_struct();

	for( uint8_t i=0; i<(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/5; i++){
		add_value = (FIRST_COUNTER+PWM_VALUE>=(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2)?(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2-FIRST_COUNTER:PWM_VALUE;
		minus_value = (SECOND_COUNTER-PWM_VALUE<=(MEANDR_TIMER_TICKS+DEADTIME_TICKS)/2)?SECOND_COUNTER-(MEANDR_TIMER_TICKS+DEADTIME_TICKS)/2:PWM_VALUE;
		if (minus_value==0 || add_value==0){
//			led1_on();
			return;
		}
		FIRST_COUNTER += add_value;
		SECOND_COUNTER -= minus_value;
		HAL_Delay(1);
	}
//	led1_off();
//	GPIOA->BSRR |= GPIO_BSRR_BR_7;
};

void pwm_lock(){
//	led1_on();
//	led2_on();
	FIRST_COUNTER = 0;
	SECOND_COUNTER = MEANDR_TIMER_TICKS;
};
#endif
//*/
//*
// Расчет идет по счетчику в канале, который в моде=1,  здесь это 2 канал
/*
uint8_t pwm_up2(uint16_t pwm_value){
	uint8_t add_value, minus_value = 0;
	add_value = (TIM2->CCR2+pwm_value<=(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2)?pwm_value:(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2-TIM2->CCR2;
	minus_value = (TIM2->CCR1>pwm_value)?pwm_value:TIM2->CCR1;
	if (minus_value>0 && add_value>0){
		TIM2->CCR2 += add_value;
		TIM2->CCR1 -= minus_value;
		return 1;
	}
	return 0;
};
uint8_t pwm_down2(uint16_t pwm_value){
	uint8_t add_value, minus_value = 0;
	add_value = (TIM2->CCR1+pwm_value<=(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2)?pwm_value:(MEANDR_TIMER_TICKS-DEADTIME_TICKS)/2-TIM2->CCR1;
	minus_value = (TIM2->CCR2>pwm_value)?pwm_value:TIM2->CCR2;
	if (minus_value>0 && add_value>0){
		TIM2->CCR1 += add_value;
		TIM2->CCR2 -= minus_value;
		return 1;
	}
	return 0;
};
//*/

extern ADC_HandleTypeDef hadc1;

void pwm_test(void){
//	GPIOA->BSRR |= GPIO_BSRR_BS_6;
//	GPIOA->BSRR |= GPIO_BSRR_BR_7;
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	for(int j=0; j<50; j++){
		for(int i=0; i<500 && pwm_up(PWM_VALUE); i++){
			HAL_Delay(10);
		}
//		GPIOA->BSRR |= GPIO_BSRR_BS_7 | GPIO_BSRR_BR_6;
		for(int i=0; i<500 && pwm_down(PWM_VALUE); i++)
			  HAL_Delay(10);
		soft_start();
	}
}


#ifdef STM32F750
void PWM_init(){
//		Включаем тактирование портов вывода таймера
		RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN;
//		Включаем тактирование Таймера 4
		RCC->APB1HENR |= RCC_APB1LENR_TIM4EN;
// Настраиваем порты вывода D12, D13, D14
		GPIOD->OSPEEDR = GPIO_OSPEEDR_OSPEED12; // Максимальные скорости для порта:  0b11 - Very high speed
		GPIOD->OSPEEDR = GPIO_OSPEEDR_OSPEED13;
		GPIOD->OSPEEDR = GPIO_OSPEEDR_OSPEED14;

		GPIOD->MODER &= ~GPIO_MODER_MODE12; //будет альтернативная функция
		GPIOD->MODER |= GPIO_MODER_MODE12_1;
		GPIOD->MODER &= ~GPIO_MODER_MODE13; //будет альтернативная функция
		GPIOD->MODER |= GPIO_MODER_MODE13_1 ;
		GPIOD->MODER &= ~GPIO_MODER_MODE14;
		GPIOD->MODER |= GPIO_MODER_MODE14_1; //будет альтернативная функция

		// Альтернативная функция - для работы портов D12, D13, D14 с таймером
		GPIOD->AFR[1]  &= ~GPIO_AFRH_AFRH4;
		GPIOD->AFR[1]  &= ~(0x0F << 4*4);    //  Одно и то же
		GPIOD->AFR[1]  |= (Alternate_Function << 4*4); // 12 port, сдвиг на 4 полубайта влево

		GPIOD->AFR[1]  &= ~(0x0F << 4*5);
		GPIOD->AFR[1]  |= (Alternate_Function << 4*5); // 13 port, сдвиг на 5 полубайт влево

		GPIOD->AFR[1]  &= ~(0x0F << 4*6);
		GPIOD->AFR[1]  |= (Alternate_Function << 4*6); // 14 port, сдвиг на 6 полубайт влево
// Настраиваем таймер TIM4
		NVIC_EnableIRQ(TIM4_IRQn);
//		TIM4->PSC=PWM_PSC;
//		TIM4->ARR=MEANDR_TIMER_TICKS;
}
#endif
