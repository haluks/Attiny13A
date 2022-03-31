/*
 * software_i2c.c
 *
 * Created: 28.03.2022 23:06:34
 * Author : haluk
 */ 
//#define F_CPU 1200000ul //1.2MHz fuse: lowV 6A, highV FF 
//#define F_CPU 9600000ul //9.6MHz fuse: lowV 7A, highV FF //i2c freq 100KHz
//9.6 ile calismasi icin kapalý olan delaylar acilmali
#include <avr/io.h>
#include "software_i2c.h"
//#include <util/delay.h>

i2c_cmd_t i2c_task=0;
uint8_t i2c_bas=0,i2c_son=0,i2c_len=0;
uint8_t i2c_ring[I2C_Boyut];

void i2c_init(){
	SDA_HIGH;
	SCL_HIGH;
	I2C_OUT;
}
void i2c_start(){
	SCL_HIGH;
	SDA_LOW;
	SCL_LOW;
	i2c_transfer();
}
void i2c_stop(){
	SDA_LOW;
	SCL_HIGH;
	SDA_HIGH;
}
void i2c_transfer(){
	uint8_t I2CDR=0;
	i2c_son=0;
	do{
		I2CDR=i2c_ring[i2c_son++];
		for( uint8_t i=0;i<8;i++){
			if (I2CDR&0x80){
				SDA_HIGH;
				}else{
				SDA_LOW;
			}
			//_delay_us(3);
			SCL_HIGH;
			while (!(PINB&(1<<SCL)));
			I2CDR<<=1;
			//_delay_us(4);
			SCL_LOW;
		}
		SDA_IN;
		//_delay_us(3);
		SCL_HIGH;
		I2CDR=I2C_PIN;//ack kontrol nack ise cikar
		//_delay_us(4);
		SCL_LOW;
		//_delay_us(4);
		SDA_OUT;
	}while (!(I2CDR&I2C_NACK)&&(i2c_son<i2c_bas)&&(i2c_task== I2C_WRITE));//adres yazýldý sonrasýnda yazma icin devam eder okumada alta gecer
	
	if ((i2c_task== I2C_READ)&&(!(I2CDR&I2C_NACK))){//okuma icin devam eder nack ise girmez
		do{
			SDA_IN;
			//_delay_us(7);
			for( uint8_t i=0;i<8;i++){
				I2CDR<<=1;
				//_delay_us(3);
				SCL_HIGH;
				if (I2C_SDA_HIGH){
					I2CDR|=0x01;
					}else{
					I2CDR&=~0x01;
				}
				//_delay_us(4);
				SCL_LOW;
			}
			i2c_ring[i2c_bas++]=I2CDR;
			SDA_OUT;
			if (i2c_bas!=i2c_len){
				SDA_LOW;//ack gonder
				}else{
				SDA_HIGH;//son veride nack gonder
			}
			//_delay_us(3);
			SCL_HIGH;
			//_delay_us(4);
			SCL_LOW;
		}while (i2c_bas!=i2c_len);
	}	
	i2c_stop();
}
void i2c_adr(uint8_t adr,i2c_cmd_t cmd){
	i2c_bas=0;
	i2c_task=cmd;
	i2c_ring[i2c_bas++]=((adr<<1)|cmd);
}
void i2c_data(uint8_t data){
	i2c_ring[i2c_bas++]=data;
}
void i2c_end(){
	
	i2c_start();
}
void i2c_send_data(uint8_t adr, uint8_t data){
	i2c_adr(adr, I2C_WRITE);
	i2c_data(data);
	i2c_end();
}
void i2c_send(uint8_t adr, uint8_t* str, uint8_t len){
	i2c_adr(adr, I2C_WRITE);
	for (uint8_t i=0;i<len;i++){
		i2c_data( str[i]);
	}
	i2c_end();
}
void i2c_read(uint8_t adr, uint8_t len){	
	i2c_len=len+1;
	i2c_adr(adr, I2C_READ);	
	i2c_start();
}
uint8_t i2c_gelen(){
	return i2c_bas-i2c_son;	
}
uint8_t i2c_oku(){
	return i2c_ring[i2c_son++];
}