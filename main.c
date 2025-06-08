#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <uart.h>
#include <console.h>
#include <generated/csr.h>

#define delay(n) for(volatile int i = n; i; i--){ }

static void my_print_string(char *msg)
{
	int count = 0;
	for (int i = 0; msg[i]; i++) {
		while (uart_txfull_read()) leds_out_write(count++); // Wait for UART ready
		uart_write(msg[i]);
		//uart_ev_pending_write(UART_EV_TX); // Clear TX event
	}
}

static char msg[100];

int main(void)
{
    leds_out_write(0);
#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
    leds_out_write(1);
	uart_init();
    leds_out_write(2);

	uart_write('H');
    leds_out_write(4);
	
	//puts("Hello world!\n");	
	my_print_string("Hello world!\r\nTest\r\n");
    leds_out_write(8);

	my_print_string("Hello world!\r\n");
        
    int i = 1, d = 1;
	while(1) {
            i += d;
            if (i == 8 || i == 1) d = -d;
            leds_out_write(1 << i);
			snprintf(msg, 100, "loop! %d\r\n", i);
            my_print_string(msg);
            delay(1000000);
	}

	return 0;
}

