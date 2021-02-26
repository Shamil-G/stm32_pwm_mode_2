/*
 * adc_inject.c
 *
 *  Created on: Feb 18, 2021
 *      Author: Shamil Gusseynov
 */

#include "main.h"
#include <main-pwm.h>
#include "dev-include\adc-inject.h"

#define ADC_16Bit

#ifdef ADC_16Bit
#define ADC_COEFF 65536
#else
#define ADC_COEFF 4096
#endif

#define reference_voltage 3.26
#define TARGET_VALUE 1.5
#define PRECISION_VALUE 0.005
#define TARGET_VALUE_MAX TARGET_VALUE + TARGET_VALUE * PRECISION_VALUE
#define TARGET_VALUE_MIN TARGET_VALUE + TARGET_VALUE - PRECISION_VALUE
#define FAULT_MAX_VALUE 3
#define FAULT_MIN_VALUE 0.06
#define MAX_CURRENT
#define MIN_INPUT_VOLTAGE 0.5
#define MAX_INPUT_VOLTAGE 2.5
#define FAULT result.output_fault<FAULT_MIN_VALUE || result.output_fault>FAULT_MAX_VALUE

struct Result result;

extern ADC_HandleTypeDef hadc1;

inline void init_adc_struct(void){
	result.first_started=0;
	result.faults=0;
}

void HAL_ADCEx_InjectedErrorCallback(ADC_HandleTypeDef* hadc){
	result.input_voltage=0;
	result.output_voltage=0;
	result.output_current=0;
	result.output_fault=-1;
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	result.adc_value[0] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
	result.adc_value[1] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_2);
	result.adc_value[2] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_3);
	result.adc_value[3] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_4);
	result.input_voltage=((float)result.adc_value[0])*reference_voltage/ADC_COEFF;
	result.output_voltage=((float)result.adc_value[1])*reference_voltage/ADC_COEFF;
	result.output_current=((float)result.adc_value[2])*reference_voltage/ADC_COEFF;
	result.output_fault = ((float)result.adc_value[3])*reference_voltage/ADC_COEFF;
	if(FAULT_LOW){
		result.faults=(result.faults<CNT_FAULT)?result.faults+1:CNT_FAULT;
		if(result.faults==CNT_FAULT) {
			result.first_started=0;
			result.faults=0;
			pwm_lock();
			return;
		}
	}
	if(result.output_voltage<TARGET_VALUE_MIN){
		pwm_up(PWM_VALUE);
		return;
	}
	if( result.output_voltage>TARGET_VALUE_MAX ||
		result.output_current>MAX_CURRENT){
		pwm_down(PWM_VALUE);
		return;
	}
	if( result.output_current>MAX_CURRENT){
		pwm_down(PWM_VALUE*100);
		return;
	}
}
