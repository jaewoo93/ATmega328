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
	UCSR0A |= _BV(U2X0);		// 비동기 2배속 모드
	
	UBRR0H = 0x00;				// 통신 속도 보울(Baud rate) 설정
	UBRR0L = 207;
	
	// 비동기(비트 6, 7, 0), 8비트 데이터(비트 1, 2), 패리티 없음(비트 4, 5), 1비트 정지 비트(비트 3) 모드
	UCSR0C |= 0x06;							// 0000 0110
	UCSR0B |= _BV(RXEN0);					// 수신 가능
	UCSR0B |= _BV(TXEN0);					// 송신 가능
}

unsigned char UART_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)) );			// 데이터 수신 대기. UCSR0A 7번비트 RXC0 값 검사
	return UDR0;
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );		// 데이터 송신 대기. UCSR0A 5번비트 UDRE0 값 검사
	UDR0 = data;							// 데이터 전송
}

int main(int argc, char *argv[])
{
	unsigned char data;
	
	UART_INIT();							// UART 통신 초기화
	while(1)
	{
		data = UART_receive();				// 데이터 수신
		UART_transmit(data);				// 수신된 문자를 에코 백
	}
	
	return 0;
}
