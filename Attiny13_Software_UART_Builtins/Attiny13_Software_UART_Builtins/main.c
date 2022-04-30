/*
 * Attiny13_Software_UART_Builtins.c
 *
 * Created: 30.04.2022 18:50:12
 * Author : haluk
 */ 

#include <avr/io.h>
#include "software_uart.h"


int main(void){
	DDRB|=(1<<4);
	uart_basla();
	uart_gonder('A');
	while (1) {
		if (uart_gelen()){
			uart_gonder(uart_oku());
		}
	}
}