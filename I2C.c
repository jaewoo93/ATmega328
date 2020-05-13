#include "I2C.h"

void I2C_init(void)
{
	DDRC |= (1 << I2C_SCL);					// SCL ���� ������� ����
	DDRC |= (1 << I2C_SDA);					// SDA ���� ������� ����
	
	TWBR = 32;								// I2C Ŭ�� ���ļ� ���� 200KHz
	
	TWCR = (1 << TWEN) | (1 << TWEA);		// I2C Ȱ��ȭ, ACK ���
}

void I2C_start(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );		// ���� �Ϸ� ���
}

void I2C_transmit(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );		// ���� �Ϸ� ���
}

uint8_t I2C_receive(void)
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	
	while( !(TWCR & (1 << TWINT)) );		// ���� �Ϸ� ���
	
	return TWDR;
}

void I2C_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) | _BV(TWEA);
}