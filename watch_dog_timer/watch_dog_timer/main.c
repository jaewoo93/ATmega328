/*
 * watch_dog_timer.c
 *
 * Created: 2020-05-20 오전 11:01:08
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/wdt.h>
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

void UART_print16bitNumber(uint16_t no)
{
	char numString[6] = "0";
	int i, index = 0;
	
	if(no > 0){
		for(i = 0; no != 0 ; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] = '\0';
		index = i - 1;
	}
	
	for(i = index; i >= 0; i--)
	UART_transmit(numString[i]);
}

void UART_printString(char *str)
{
	for(int i = 0; str[i]; i++)
	UART_transmit(str[i]);
}

int main(void)
{
    wdt_enable(WDTO_8S);		// 와치독 타이머 만료 시간을 8초로 설정
	UART_INIT();				// UART 통신 초기화
	UART_printString("** Initialization ** \n");
	
	uint16_t count = 0;
	
    while (1) 
    {
		_delay_ms(1000);		// 1초에 한번 카운트 출력
		
		count++;
		UART_printString("Count : ");
		UART_print16bitNumber(count);
		UART_transmit('\n');
		//wdt_reset();		// 와치독 타이머 리셋
    }
}

