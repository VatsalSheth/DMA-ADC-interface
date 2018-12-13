/*
 * uart.c
 *
 *  Created on: Nov 14, 2018
 *      Author: jains
 */

#include "uart.h"

void uart_init()
{
	/*Clock Enable*/
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; //turn ON clock gate
		//SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
		SIM->SOPT2 |= 0x4000000; //FLL or PLL Clock

		UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

		UART0->C1 &= ~(UART0_C1_M_MASK | UART0_C1_PE_MASK);
		UART0->BDH &= ~UART_BDH_SBNS(1);

		/*Baud Config*/
		uint16_t SBR=((SystemCoreClock)/(OSR_VAL*BAUD_RATE));
		UART0_C4=UART0_C4_OSR(0x0F);
		UART0_BDH= /*0x00;*/((SBR>>8)& UART0_BDH_SBR_MASK);
		UART0_BDL= /*146;*/(SBR & UART0_BDL_SBR_MASK);
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; //enable PORTA
		PORTA_PCR1 = PORT_PCR_MUX(2);
		PORTA_PCR2 = PORT_PCR_MUX(2);

		/*Enable Transmitter, Receiver and Receiver interrupt*/
		UART0_C2 |= (UART0_C2_TE_MASK); //transmitter enable, Receiver enable

		UART0_S1 &= ~(UART0_S1_TDRE_MASK | UART0_S1_TC_MASK);

		UART0_C2 |= UART0_C2_TIE_MASK;

		NVIC->ISER[0] |= (uint32_t)(1UL << (((uint32_t)(int32_t)UART0_IRQn) & 0x1FUL));
}

void uart_putc(uint8_t data)
{
	buffinsert(ring_tx, (int8_t)data);
}

void UART0_IRQHandler(void)
{
	__disable_irq();	//Disable all interrupts
	if((UART0_S1 & UART0_S1_TDRE_MASK))
	{
		tx_flag = 1;
		UART0_C2 &= ~UART0_C2_TIE_MASK;
	}

	__enable_irq();		//Enable all interrupts
	return;
}

void print_string(char* to_print)
{
	while(*to_print != '\0')
	{
		uart_putc(*(to_print++));
	}
}

//print float number upto 2 decimal points
void print_float(float a)
{
    uint8_t i=2;
    print_int((uint16_t)a);
    uart_putc('.');
    while(i!=0)
    {
        a = a - (uint16_t)a;
        a *= 10;
        uart_putc(0x30 + (uint16_t)a);
        i-=1;
    }
}

void print_int(uint16_t to_print_int)
{
	uint8_t i = 5, valid = 0;
	uint16_t div = 10000;

	if((to_print_int & 0x8000) == 0x8000)
	{
		uart_putc('-');
		to_print_int = (~to_print_int) + 1;
	}

	if(to_print_int > max_local)
	{
		max_local = to_print_int;
	}

	while(i != 0)
	{
		if(to_print_int/div || valid == 1 || i==1)
		{
			valid = 1;
			uart_putc(0x30 + to_print_int/div);
		}
		to_print_int = to_print_int % div;
		div = div/10;
		i--;
	}
}
