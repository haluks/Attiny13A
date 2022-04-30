/*
 * software_uart.h
 *
 * Created: 16.03.2022 15:09:54
 *  Author: haluk
 */ 


#ifndef SOFTWARE_UART_H_
#define SOFTWARE_UART_H_

#define F_CPU 9600000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/builtins.h>

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
#define UART_Rx_Boyut 8
#define UART_Rx_Mask (UART_Rx_Boyut-1)


//degerler 9,6MHz için ayarlandý
/*// BAUDRATE		9600
#define TX_DELAY		978
#define RX_DELAY		975
#define RX_STOP_DELAY	1350
#define RX_YARIM_DELAY	468
//*/
/*//BAUDRATE		19200
#define TX_DELAY		480
#define RX_DELAY		476
#define RX_STOP_DELAY	635
#define RX_YARIM_DELAY	226
//*/
/*//BAUDRATE		38400
#define TX_DELAY		234
#define RX_DELAY		230
#define RX_STOP_DELAY	280
#define RX_YARIM_DELAY	98
//*/
/*//BAUDRATE		57600
#define TX_DELAY		152
#define RX_DELAY		148
#define RX_STOP_DELAY	165
#define RX_YARIM_DELAY	55
//*/
///BAUDRATE		115200
#define TX_DELAY		69
#define RX_DELAY		65
#define RX_STOP_DELAY	75
#define RX_YARIM_DELAY	12
//*/
////////////////////////////////////////////////////
void uart_basla();
uint8_t uart_oku();
void uart_gonder(uint8_t uData);
void uart_dizi(const char *str);
uint8_t uart_gelen();
void uart_dizi_al( char *stri);

#endif 