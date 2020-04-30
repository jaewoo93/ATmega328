/*
 * read_volume.c
 *
 * Created: 2020-04-29 오후 8:51:21
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include <stdio.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);		// 2배속 모드
	
	UBRR0H = 0x00;
	UBRR0L = 207;		// Baud rate(9600) 설정
	
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);		// 송수신 가능
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );		// 송신 가능 대기
	UDR0 = data;		// 데이터 전송
}

void UART_transmit_string(char *str)
{
	while(*str)		// 문자열의 끝 검사
	{
		UART_transmit(*str);		// 문자 단위 전송
		str++;
	}
	
	UART_transmit('\n');		// 줄바꿈 문자 추가
}

void ADC_INIT(void)
{
	ADMUX |= 0x40;		// AVCC를 기준전압으로 선택  0100 0000
	
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);		// 분주비 128 설정 ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	0000 0111	
	ADCSRA |= (1 << ADEN);		// ADC 활성화
	ADCSRA |= (1 << ADATE);		// 자동 트리거 모드
	
	ADMUX |= ((ADMUX & 0xE0) | channel);		// 채널 선택.  0xE0 = 1110 0000
	ADCSRA |= (1 << ADSC);		// 변환 시작
}

int read_ADC(unsigned char channel)
{
	while(!(ADCSRA & (1 << ADIF)));		// 변환 종료 대기. AD 변환이 종료되고 데이터 레지스터가 업데이트 되면 1로 세트
	return ADCW;		// 10비트 값을 변환	return ADCW;
}

void int_to_string(int n, char *buffer)
{
	sprintf(buffer, "%04d", n);		// ADC 값을 0으로 채워진 4자리 문자열로 변환
	buffer[4] = '\0';
}

int main(void)
{
    int read;
	char buffer[5];
	
	
	UART_INIT();		// UART 통신 초기화
	ADC_INIT();		// AD 변환기 초기화
	
	while(1)
	{
		read = read_ADC(0);		// 가변저항 값 읽기
		int_to_string(read, buffer);		// 정수값을 문자열로 변환
		UART_transmit_string(buffer);		// 문자열을 UART로 전송
		
		_delay_ms(1000);		// 1초에 한 번 읽음
	}
}


