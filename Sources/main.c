#include "main.h"


void main()
{
	uart_init();
	dma_init();
	adc_init();

	/*Enable IRQ*/
	__enable_irq();

	ring_tx = buffinit(500);
	print_string("Hey there!!!\n\r");
	while(1)
	{
		if(tx_flag == 1 && ring_tx -> count != 0)
		{
			tx_flag = 0;
			buffread(ring_tx, (int8_t *)&tx_char);
			UART0 -> D = tx_char;
			UART0_C2 |= UART0_C2_TIE_MASK;
		}
		if(swap_flag == 1)
		{

		}
		app_update();
	}
}

void app_update()
{
	int16_t data;
	data = double_buffer[app_addr+app_cnt];
	print_int(data);
	uart_putc('\n');
	uart_putc('\r');
}
