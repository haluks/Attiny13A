/*
 * software_i2c.h
 *
 * Created: 28.03.2022 23:07:15
 *  Author: haluk
 */ 


#ifndef SOFTWARE_I2C_H_
#define SOFTWARE_I2C_H_
#define I2C_Boyut		8
#define ACK				0x00
#define NACK			0x01
#define SDA				PORTB3
#define SCL				PORTB4
#define I2C_PORT		PORTB
#define I2C_DDR			DDRB
#define I2C_PIN			PINB
#define I2C_OUT			I2C_DDR|=((1<<SDA)|(1<<SCL))
#define SDA_OUT			I2C_DDR|=(1<<SDA)
#define SDA_IN			I2C_DDR&=~(1<<SDA)

#define SCL_LOW			I2C_PORT&=~(1<<SCL)
#define SCL_HIGH		I2C_PORT|=(1<<SCL)
#define SDA_LOW			I2C_PORT&=~(1<<SDA)
#define SDA_HIGH		I2C_PORT|=(1<<SDA)

#define I2C_SDA_HIGH	I2C_PIN&(1<<SDA)
#define I2C_NACK	(1<<SDA)

typedef enum {
	I2C_WRITE		=0,
	I2C_READ		=1,
}i2c_cmd_t;

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write_addr();
void i2c_transfer();
void i2c_adr(uint8_t adr,i2c_cmd_t cmd);
void i2c_data(uint8_t data);
void i2c_end();
void i2c_send_data(uint8_t adr, uint8_t data);
void i2c_send(uint8_t adr, uint8_t* str,uint8_t len);
void i2c_read(uint8_t adr, uint8_t len);
uint8_t i2c_gelen();
uint8_t i2c_oku();

#endif /* SOFTWARE_I2C_H_ */
