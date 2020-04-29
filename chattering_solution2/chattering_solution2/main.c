/*
 * chattering_solution2.c
 *
 * Created: 2020-04-29 오후 2:48:51
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);		// 2배속 모드
	
	UBRR0H = 0x00;
	UBRR0L = 207;				// 통신 속도(보울) 설정
	
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	
	UCSR0B |= _BV(RXEN0);		// 송수신 가능
	UCSR0B |= _BV(TXEN0);
}

void UART_transmit(unsigned char data)
{
	 while( !(UCSR0A & (1<<UDRE0)) );		// 송신 가능 대기
	 UDR0 = data;
}

void INIT_PORT(void)
{
	DDRB = 0x20;		// PB5을 출력으로 설정
	PORTB = 0x00;		// LED는 꺼진 상태에서 시작
	
	DDRD = 0x00;		// 버튼을 입력으로 설정
	PORTD = 0x04;		// PD2 풀업 저항 사용
}

unsigned char button_pressed(void)
{
	if(bit_is_clear(PIND, PIND2))			// 첫 번째 검사
	{
		_delay_ms(10);
		if(bit_is_clear(PIND, PIND2))		// 두 번째 검사
			return 1;
	}
	return 0;
}

int main(void)
{
	UART_INIT();		// UART 통신 초기화
	INIT_PORT();		// 포트 설정
	int state = 0;		// 현재 버튼 상태 
	
	while(1)
	{
		if(button_pressed())
		{
			// 눌러지지 않은 상태에서 눌러진 상태로 바뀔 때마다 '*' 문자 전송
			if(state == 0)	UART_transmit('*');
			state = 1;			// 버튼이 눌러진 상태로 표시
			PORTB = 0x20;		// LED 끄기
		}
		else
		{
			state = 0;			// 버튼이 눌러지지 않은 상태로 표시
			PORTB = 0x00;		// LED 끄기
		}
	}  
	
	return 1;
}



