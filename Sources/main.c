
/***********************************************************************************

* @main.c

* @This file contains main commands and option decode logic. Peripherals are initialized

* and the local peak is updated after each buffer interrupt. Log of each peak is displayed

* @author Vatsal Sheth & Sarthak Jain

************************************************************************************/

#include "main.h"

db_scale lookup[]=
			 {
					 {0,-26.02}, {1,-20},{2,-16.47},{3,-13.97},{4,-12.04},{5,-10.45},
					 {6,-9.11},{7,-7.95},{8,-6.93},{9,-6.02},{10,-5.19},
					 {11,-4.43}, {12,-3.74},{13,-3.09},{14,-2.49},{15,-1.19},{16,-1.41},
					 {17,-0.91},{18,-0.44},{19,0}
			 };

void main()
{
	uart_init();
	adc_init();
	dma_init();

	/*Enable IRQ*/
	__enable_irq();

	ring_tx = buffinit(500);
	print_string("Hey there!!!\n\r");

	while(1)
	{
		if(tx_flag == 1 && ring_tx -> count != 0)	//Check if transmit buffer ready to send data
		{
			tx_flag = 0;
			buffread(ring_tx, (int8_t *)&tx_char);
			UART0 -> D = tx_char;
			UART0_C2 |= UART0_C2_TIE_MASK;
		}

		if(swap_flag == 1)
		{
			swap_flag = 0;
			if(max_local >= max_global)		//Check for new peak
			{
				max_global = max_local;
			}
			else
			{
				max_global = 0.9 * max_global;		//Decay of 0.9x to previous peak
			}
			db_global = my_log10(max_global);
			if(db_global != -1)
			{
				db_global = db_global * (-1);
				print_string("\r\n//////////////////////////////////////////////////\r\n");
				print_string("Max value : -");
				print_float(db_global);
				print_string(" db and ");
				print_int(max_global);
				print_string(" value\n\r");
			}
			max_local = double_buffer[app_addr];		//first address data asigned to local peak
		}

		if(app_cnt<64)
		{
			app_update();
		}
	}
}

float my_log10(uint16_t x)		//Returns log value of ADC readings
{
	uint8_t i=0;

	x = x/1639;
	for(i=0; i<20; i++)
	{
		if(x == lookup[i].x)
		{
			return lookup[i].db;
		}
	}
	return -1;
}

void app_update()		//data updated from buffer
{
	int16_t data;
	data = double_buffer[app_addr+app_cnt];
	app_cnt++;
	print_int((uint16_t)data);
	uart_putc('\n');
	uart_putc('\r');
}
