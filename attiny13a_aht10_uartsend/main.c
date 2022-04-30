/*
 * attiny13a_aht10_uartsend.c
 *
 * Created: 28.03.2022 23:23:06
 * Author : haluk
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "software_uart.h"
#include "software_i2c.h"
#define aht_adr	0x38//aht10adres
#define aht_res	0xBA//soft reset
//uint8_t aht_init[]={0xE1,0x08,0x00};//init
//uint8_t aht_trig[]={0xAC,0x33,0x00};//ölçüm baþlatma
int main(void)
{
   uart_basla();
   //DDRB|=(1<<4);
   i2c_init();
   i2c_send_data(aht_adr,aht_res);//soft reset
_delay_loop_2(100);
   i2c_adr(aht_adr, I2C_WRITE);
   i2c_data(0xE1);
   i2c_data(0x08);
   i2c_data(0x00);
   i2c_end();
    while (1)  {
		
		 i2c_adr(aht_adr, I2C_WRITE);
		 i2c_data(0xAC);
		 i2c_data(0x33);
		 i2c_data(0x00);
		 i2c_end();
		 _delay_loop_2(10000);
		i2c_read(aht_adr,6);// veri okuma
		
		if (i2c_gelen()){//okunan veri alýndý
			uint8_t aht_data[7];
			uint32_t aht_temp=0;
			uint16_t sicaklik=0;
			uint32_t aht_hum=0;
			uint16_t nem=0;			
			aht_data[0]=i2c_oku();
			aht_data[1]=i2c_oku();
			aht_data[2]=i2c_oku();
			aht_data[3]=i2c_oku();
			aht_data[4]=i2c_oku();
			aht_data[5]=i2c_oku();
						
			aht_hum=(((uint32_t)aht_data[1]<<16)|((uint16_t)aht_data[2]<<8)|aht_data[3])>>4;
			aht_temp=((uint32_t)(aht_data[3]&0x0F)<<16)|((uint16_t)aht_data[4]<<8)|(aht_data[5]);
		
			sicaklik=((aht_temp*625)>>15)- 5000;//virgulden sonrasý icin deðiþiklik yaptim float yer kapliyor
			uint8_t binler=sicaklik/1000;
			uint8_t yuzler=(sicaklik/100)%10;
			uint8_t onlar=(sicaklik/10)%10;
			uint8_t birler=sicaklik%10;
			uart_dizi("sic:");
			uart_gonder(0x30|binler);
			uart_gonder(0x30|yuzler);
			uart_gonder(0x2C);
			uart_gonder(0x30|onlar);
			uart_gonder(0x30|birler);
			nem=(aht_hum*625)>>16;//virgulden sonrasý icin deðiþiklik yaptim float yer kapliyor
			binler=nem/1000;
			yuzler=(nem/100)%10;
			onlar=(nem/10)%10;
			birler=nem%10;
			uart_dizi(" nem:");
			uart_gonder(0x30|binler);
			uart_gonder(0x30|yuzler);
			uart_gonder(0x2C);
			uart_gonder(0x30|onlar);
			uart_gonder(0x30|birler);
			uart_gonder('\n');
		
		}
		//_delay_loop_2(20000);*/
    }
}

