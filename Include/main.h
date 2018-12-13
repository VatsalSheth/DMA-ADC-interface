/*
/***********************************************************************************

* @main.h

* @This header file includes all the function header files and it had variable and

* function declaration used in main.c

*

* @author Vatsal Sheth & Sarthak Jain

************************************************************************************/


#include <math.h>
#include "uart.h"
#include "adc.h"
#include "dma.h"

uint8_t tx_char;
int16_t max_global;
float db_global;

typedef struct
{
	uint8_t x;
	float db;
}db_scale;

float my_log10(uint16_t x);
void app_update();
