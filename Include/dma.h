/*
 * dma.h
 *
 *  Created on: Dec 8, 2018
 *      Author: Vatsal Sheth
 */

#ifndef DMA_H_
#define DMA_H_
#include "MKL25Z4.h"
#include "stdint.h"

uint32_t addr[10];
uint8_t tmp=0;
int16_t double_buffer[128];
uint32_t app_addr = 64, dma_addr = 0;
uint8_t swap_flag, app_cnt;

void dma_init();

#endif /* DMA_H_ */
