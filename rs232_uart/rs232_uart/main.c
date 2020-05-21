/*
 * rs232_uart.c
 *
 * Created: 2020-05-21 오후 1:51:45
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

void UART_INIT(void) {
	UCSR0A |= _BV(U2X0);

	UBRR0H = 0x00;
	UBRR0L = 207;

	UCSR0C |= 0x06;
	
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

int main(int argc, char *argv[])
{
    char data = '0';
	
	UART_INIT();		// UART 통신 초기화
    while (1) 
    {
		UART_transmit(data++);		// 카운터값을 컴퓨터로 전달
		UART_transmit('\n');		// 줄바꿈
		
		if(data > '9')	data = '0';
		
		_delay_ms(1000);
    }
	
	return 0;
}