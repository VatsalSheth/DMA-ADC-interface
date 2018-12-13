/*
 * uart.h
 *
 *  Created on: Nov 14, 2018
 *      Author: jains
 */

#include "bufferprop.h"

#define BAUD_RATE 115200
//#define BAUD_RATE 9600
#define OSR_VAL 0x0F
#define SIM_SOPT2_FLLCLK 4000000U


void uart_init();
void uart_putc(uint8_t);
void print_string(char* to_print);
void print_int(uint16_t to_print_int);
void print_float(float a);

ring_wbells *ring_tx;
int16_t max_local;

volatile int8_t tx_flag;





