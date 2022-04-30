/*
 * software_uart.c
 *
 * Created: 16.03.2022 15:10:12
 *  Author: haluk
 */ 
#include "software_uart.h"

volatile uint8_t rx_bas=0,rx_son=0;
volatile uint8_t rx_ring[UART_Rx_Boyut];
volatile uint8_t veri=0;
ISR(PCINT0_vect){		
	if (RX_PIN_LOW){//start kontrol pin "0" ise	
		//ilk bit i�in bekleme
		__builtin_avr_delay_cycles(RX_YARIM_DELAY);
		for (uint8_t i=0;i<8;i++){			
			//her bir bit i�in gereken bekleme
			__builtin_avr_delay_cycles(RX_DELAY);
			veri>>=1;//ge�ici de�i�ken kayd�r�ld�
		
			if (RX_PIN_HIGH){// hat kontrol� yap�l�yor
				veri|=0x80;//"1" ise "1"			
				//PORTB|=(1<<4);				
			}else{//zamanlama dengesi i�in eklendi
				veri&=~0x80;//"0" ise "0" yaz�l�yor	
				//PORTB&=~(1<<4);			
			}//*/		
		}
		__builtin_avr_delay_cycles(RX_STOP_DELAY);
		// hemen sonras�nda g�nderime ge�memek i�in
		rx_bas=(rx_bas+1) & UART_Rx_Mask;//ring buffer indis artt�
		rx_ring[rx_bas]=veri;//ring buffer yaz�ld�
	}
}

void uart_basla(){
	//cli();
	RX_HIGH;
	RX_IN;//pin giri� yap�ld�
	TX_HIGH;
	TX_OUT;//pin ��k�� yap�ld�
	PCMSK|=(1<<RX_);//rx pini i�in kesme ayarland�
	GIMSK|=(1<<PCIE);	
	sei();
}

uint8_t uart_oku(){

	rx_son=(rx_son+1) & UART_Rx_Mask;
	return rx_ring[rx_son];
}
void uart_gonder(uint8_t uData){	
	__builtin_avr_delay_cycles(TX_DELAY);
	TX_LOW;//start ko�ulu
	//start i�in gerekli bekleme
	__builtin_avr_delay_cycles(TX_DELAY);
	cli();//kesmeler kapat�ld�
	for (uint8_t i=0;i<8;i++){//8 bit i�in tekrar
		if (uData&0x01){//ilk bit kontrol edildi
			TX_HIGH;//ilk bit "1" ise "1"
			}else{
			TX_LOW;//ilk bit "0" ise "0"
			//burada else gerekli de�il ama
			//"0" olmas� durumunda zamanlama yani gerekli �evrim de�i�mesin diye eklendi			
		}
		__builtin_avr_delay_cycles(TX_DELAY);
		//her bir bit i�in gerekli bekleme
		uData>>=1;//veri bir bit kayd�r�ld�
	}	
	TX_HIGH;//stop ko�ulu	
	//stop i�in gereken bekleme
	__builtin_avr_delay_cycles(TX_DELAY);
	sei();//kesmeler a��ld�
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
		stri[poz++]=uart_oku();
		//poz++;
	} while (!(rx_bas==rx_son));
	stri[poz]='\0';
}