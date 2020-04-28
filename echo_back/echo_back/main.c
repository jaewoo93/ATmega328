/*
 * echo_back.c
 *
 * Created: 2020-04-28 오후 4:07:05
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);		// 2��� ���
	
	UBRR0H = 0x00;				// ��� �ӵ�(Baud rate) ����
	UBRR0L = 207;
	
	// �񵿱�, 8��Ʈ ������, �и�Ƽ ����, 1��Ʈ ���� ��Ʈ ���
	UCSR0C |= 0x06;
	UCSR0B |= _BV(RXEN0);					// �ۼ��� ����
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)) );			// ������ ���� ���
	return UDR0;
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );		// �۽� ���� ���
	UDR0 = data;							// ������ ����
}

int main(int argc, char *argv[])
{
	unsigned char data;
	
	UART_INIT();
	while(1)
	{
		data = UART_receive();
		UART_transmit(data);
	}
	
	return 0;
}
