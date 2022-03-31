/*
 * software_uart.c
 *
 * Created: 16.03.2022 15:10:12
 *  Author: haluk
 */ 
//attiny13a icin yeniden duzenledim
//dusuk frekansta calistigindan 9600 ustu baudrate ile sorun yasaniyor
//bu nedenle sabit 9600 yapmaya karar verdim
// F_cpu artarsa i2c de delay gerekiyor bu da fazladan yer demek zaten hafýza sinirli 1Kb

#include "software_uart.h"
//#include <avr/interrupt.h>

volatile uint8_t rx_bas=0,rx_son=0;
volatile uint8_t rx_ring[UART_Rx_Boyut];
volatile uint8_t veri=0;
/*ISR(PCINT0_vect){
	if (RX_PIN_LOW){
		_delay_loop_2(RX_YARIM_DELAY);
		for (uint8_t i=0;i<8;i++){		
			_delay_loop_2(RX_DELAY);
			veri>>=1;
			
			if (RX_PIN_HIGH){
				veri|=0x80 ;
				//PORTB^=(1<<4);
				}else{
				veri&=~0x80 ;	
			}			
		}
		_delay_loop_2(RX_STOP_DELAY);
		rx_bas=(rx_bas+1) & UART_Rx_Mask;
		rx_ring[rx_bas]=veri;
	}
}*/

void uart_basla(){
	//cli();
	RX_HIGH;
	RX_IN;
	TX_HIGH;
	TX_OUT;
	//PCMSK|=(1<<RX_);//pb3
	//GIMSK|=(1<<PCIE);
	//sei();
}

uint8_t uart_oku(){
	
	rx_son=(rx_son+1) & UART_Rx_Mask;
	return rx_ring[rx_son];
}
void uart_gonder(uint8_t uData){	
	_delay_loop_2(TX_DELAY);
	TX_LOW;
	_delay_loop_2(TX_DELAY);
	//cli();
	for (uint8_t i=0;i<8;i++){
		if (uData&0x01){
			TX_HIGH;
			}else{
			TX_LOW;
		}
		_delay_loop_2(TX_DELAY);
		uData>>=1;
	}
	TX_HIGH;
	_delay_loop_2(TX_DELAY);
	//sei();
}
void uart_dizi(const char *str){
	while(*str){
		uart_gonder(*str++);
	}
	
}
uint8_t uart_gelen(){
	
	if (rx_son==rx_bas)	return 0;
	return 1;
}
void uart_dizi_al(char *stri){
	uint8_t poz=0;
	do{
		stri[poz]=uart_oku();
		poz++;
	} while (!(rx_bas==rx_son));
	stri[poz]='\0';
}