#ifndef _I2C_H_
#define _I2C_H_

#define I2C_SCL		PC5
#define I2C_SDA		PC4

#include <avr/io.h>

void I2C_init(void);					// I2C �ʱ�ȭ
void I2C_start(void);					// I2C ����
void I2C_transmit(uint8_t data);		// 1����Ʈ ����
uint8_t I2C_receive(void);				// 1����Ʈ ����
void I2C_stop(void);					// I2C ����

#endif