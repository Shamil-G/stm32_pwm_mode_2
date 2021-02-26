/*
 * adc-inject.h
 *
 *  Created on: 26 февр. 2021 г.
 *      Author: sguss
 */
#include "main.h"

#ifndef INC_DEV_INCLUDE_ADC_INJECT_H_
#define INC_DEV_INCLUDE_ADC_INJECT_H_

//#define uint8_t Result;

extern struct Result {
	uint32_t adc_value[4];
	uint8_t first_started;
	uint8_t faults;
	float input_voltage;
	float output_voltage;
	float output_current;
	float output_fault;
} result;

void init_adc_struct(void);

#endif /* INC_DEV_INCLUDE_ADC_INJECT_H_ */
