#ifndef _I2C_H_
#define _I2C_H_

#define I2C_SCL		PC5
#define I2C_SDA		PC4

#include <avr/io.h>

void I2C_init(void);					// I2C 초기화
void I2C_start(void);					// I2C 시작
void I2C_transmit(uint8_t data);		// 1바이트 전송
uint8_t I2C_receive(void);				// 1바이트 수신
void I2C_stop(void);					// I2C 정지

#endif