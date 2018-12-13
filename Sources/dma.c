/*
 /***********************************************************************************

* @dma.c

* @This file contains DMA initialization and the interrupt handler for DMA.

*The interrupt handler clears the DONE flag and swaps the index used for read buffer

* and DMA buffer. GPIO is toggled.

* @author Vatsal Sheth & Sarthak Jain

***********************************************************************************
* dma.c
 *
 *  Created on: Dec 8, 2018
 *      Author: Vatsal Sheth
 */
#include "dma.h"

void dma_init()
{
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;	//DMA mux clock enable
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;	//DMA controller clock enable
	SIM_SCGC5 |= SIM_SCGC5_PORTE(1);

	PORTE_PCR0 |= PORT_PCR_MUX(1);
	GPIOE_PDDR |= 0x1;

	DMAMUX0_CHCFG0 = 0x00; //Disable dma mux

	DMA_SAR0 = (uint32_t)&ADC0_RA;
	DMA_DAR0 = (uint32_t)&double_buffer[dma_addr];
		DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(128); // 128 bytes (16 bits) per transfer

		DMA_DCR0 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
					 DMA_DCR_ERQ_MASK |		// Enable peripheral request
					 DMA_DCR_CS_MASK  |		//	cycle steal enable
					 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
					 DMA_DCR_DINC_MASK|		// Set increments to destination address
					 //DMA_DCR_DMOD(4)  |     // Destination address modulo of 128 bytes
					 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits

	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL(1) | DMAMUX_CHCFG_SOURCE(40); //enable dma mux, adc0 source channel 0
	NVIC->ISER[0] |= (uint32_t)(1UL << (((uint32_t)(int32_t)DMA0_IRQn) & 0x1FUL));
}

void DMA0_IRQHandler(void)
{
	__disable_irq();	//Disable all interrupts
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;	// Clear Done Flag
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;	// Clear Done Flag
	if(DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK)
	{

	}
	DMA_DAR0 = (uint32_t)&double_buffer[dma_addr];
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(128);		// Set byte count register

	app_addr = app_addr + dma_addr;
	dma_addr = app_addr - dma_addr;
	app_addr = app_addr - dma_addr;
	swap_flag = 1;
	app_cnt = 0;
	GPIOE_PTOR |= 0x1;
	__enable_irq();		//Enable all interrupts
}
