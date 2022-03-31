/*
 * software_uart.h
 *
 * Created: 16.03.2022 15:09:54
 *  Author: haluk
 */ 


#ifndef SOFTWARE_UART_H_
#define SOFTWARE_UART_H_

#define F_CPU 1200000ul
#include <avr/io.h>
#include <util/delay_basic.h>

#define TX_DDR			DDRB
#define TX_PORT			PORTB
#define TX_PIN			PINB

#define RX_DDR			DDRB
#define RX_PORT			PORTB
#define RX_PIN			PINB

#define TX_				PORTB2
#define RX_				PORTB0
#define TX_OUT			TX_DDR|=(1<<TX_)
#define TX_HIGH			TX_PORT|=(1<<TX_)
#define TX_LOW			TX_PORT&=~(1<<TX_)
#define RX_IN			RX_DDR&=~(1<<RX_)
#define RX_HIGH			RX_PORT|=(1<<RX_)
#define RX_LOW			RX_PORT&=~(1<<RX_)
#define RX_PIN_HIGH		RX_PIN&(1<<RX_)
#define RX_PIN_LOW		!(RX_PIN&(1<<RX_))


/////////////////////////////////
#define UART_Rx_Boyut	2
#define UART_Rx_Mask (UART_Rx_Boyut-1)

///BAUDRATE		19200
#define TX_DELAY		13
#define RX_DELAY		12
#define RX_STOP_DELAY	1
#define RX_YARIM_DELAY	1
//*/

/*// BAUDRATE		9600
#define TX_DELAY		28
#define RX_DELAY		27
#define RX_STOP_DELAY	13
#define RX_YARIM_DELAY	8
//*/

/*//BAUDRATE		4800
#define TX_DELAY		59
#define RX_DELAY		58
#define RX_STOP_DELAY	44
#define RX_YARIM_DELAY	24
//*/
////////////////////////////////////////////////////
void uart_basla();
uint8_t uart_oku();
void uart_gonder(uint8_t uData);
void uart_dizi(const char *str);
uint8_t uart_gelen();
void uart_dizi_al( char *stri);

#endif 