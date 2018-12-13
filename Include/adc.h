/*
/*
/***********************************************************************************

* @adc.h

* @This header file includes all the function header files and it had variable and

* function declaration used in adc.c

*

* @author Vatsal Sheth & Sarthak Jain

************************************************************************************
 * adc.h
 *
 *  Created on: Dec 5, 2018
 *      Author: Vatsal Sheth
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "MKL25Z4.h"

int32_t adc_init();
int32_t adc_cal();

#endif /* ADC_H_ */
