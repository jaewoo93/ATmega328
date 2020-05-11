/*
 * eeprom_pointer.c
 *
 * Created: 2020-05-11 오전 9:12:35
 * Author : user
 */

#include <avr/io.h>
#include <avr/eeprom.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);						// 2배속 모드
	
	UBRR0H = 0x00;								// 통신 속도(보율) 설정
	UBRR0L = 207;								// 보율 = 9600
	
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	
	UCSR0B |= _BV(RXEN0);						// 송수신 가능
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)				// 1바이트 수신
{
	while( !(UCSR0A & (1<<RXC0)) );				// 데이터 수신 대기
	return UDR0;
}

void UART_transmit(unsigned char data)			// 1바이트 송신
{
	while( !(UCSR0A & (1<<UDRE0)) );			// 송신 가능 대기
	UDR0 = data;
}

void UART_printString(char *str)				// 문자열 송신
{
	for(int i=0; str[i]; i++)					// '\0' 문자를 만날 때까지 반복
	UART_transmit(str[i]);					// 바이트 단위 출력
}

void UART_print8bitNumber(uint8_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

void UART_print16bitNumber(uint16_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[6] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

void UART_print32bitNumber(uint32_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[11] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

int main(void)
{
	UART_INIT();		// UART 통신 초기화
	
	uint8_t *p1, *p2;
	p1 = 0;
	p2 = p1 + 1;		// p1 번지에 uint8_t 데이터 형의 바이트 수를 더한 값 = 1
	
	UART_printString("P1 : ");
	UART_print16bitNumber((uint16_t)p1);
	UART_printString("\n");
	UART_printString("P2 : ");
	UART_print16bitNumber((uint16_t)p2);
	UART_printString("\n");
	
	uint16_t *p3, *p4;
	p3 = 0;
	p4 = p3 + 1;		// p3 번지에 uin16_t 데이터 형의 바이트 수를 더한 값 = 2
	
	UART_printString("P3 : ");
	UART_print16bitNumber((uint16_t)p3);
	UART_printString("\n");
	UART_printString("P4 : ");
	UART_print16bitNumber((uint16_t)p4);
	UART_printString("\n");
	
    while(1);
	
	return 0;
}

