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
		//ilk bit için bekleme
		__builtin_avr_delay_cycles(RX_YARIM_DELAY);
		for (uint8_t i=0;i<8;i++){			
			//her bir bit için gereken bekleme
			__builtin_avr_delay_cycles(RX_DELAY);
			veri>>=1;//geçici deðiþken kaydýrýldý
		
			if (RX_PIN_HIGH){// hat kontrolü yapýlýyor
				veri|=0x80;//"1" ise "1"			
				//PORTB|=(1<<4);				
			}else{//zamanlama dengesi için eklendi
				veri&=~0x80;//"0" ise "0" yazýlýyor	
				//PORTB&=~(1<<4);			
			}//*/		
		}
		__builtin_avr_delay_cycles(RX_STOP_DELAY);
		// hemen sonrasýnda gönderime geçmemek için
		rx_bas=(rx_bas+1) & UART_Rx_Mask;//ring buffer indis arttý
		rx_ring[rx_bas]=veri;//ring buffer yazýldý
	}
}

void uart_basla(){
	//cli();
	RX_HIGH;
	RX_IN;//pin giriþ yapýldý
	TX_HIGH;
	TX_OUT;//pin çýkýþ yapýldý
	PCMSK|=(1<<RX_);//rx pini için kesme ayarlandý
	GIMSK|=(1<<PCIE);	
	sei();
}

uint8_t uart_oku(){

	rx_son=(rx_son+1) & UART_Rx_Mask;
	return rx_ring[rx_son];
}
void uart_gonder(uint8_t uData){	
	__builtin_avr_delay_cycles(TX_DELAY);
	TX_LOW;//start koþulu
	//start için gerekli bekleme
	__builtin_avr_delay_cycles(TX_DELAY);
	cli();//kesmeler kapatýldý
	for (uint8_t i=0;i<8;i++){//8 bit için tekrar
		if (uData&0x01){//ilk bit kontrol edildi
			TX_HIGH;//ilk bit "1" ise "1"
			}else{
			TX_LOW;//ilk bit "0" ise "0"
			//burada else gerekli deðil ama
			//"0" olmasý durumunda zamanlama yani gerekli çevrim deðiþmesin diye eklendi			
		}
		__builtin_avr_delay_cycles(TX_DELAY);
		//her bir bit için gerekli bekleme
		uData>>=1;//veri bir bit kaydýrýldý
	}	
	TX_HIGH;//stop koþulu	
	//stop için gereken bekleme
	__builtin_avr_delay_cycles(TX_DELAY);
	sei();//kesmeler açýldý
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