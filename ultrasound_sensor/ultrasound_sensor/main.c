/*
 * ultrasound_sensor.c
 *
 * Created: 2020-05-20 오후 5:26:18
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define PRESCALER 1024		// 분주비

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

void UART_printString(char *str)
{
	for(int i = 0; str[i]; i++)
	UART_transmit(str[i]);
}

void UART_print8bitNumber(uint8_t no)
{
	char numString[4] = "0";
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

void Timer_init(void)
{
	TCCR0B |= (1 << CS02) | (1 << CS00);		// 분주비 1024로 설정
}

uint8_t measure_distance(void)
{
	// 트리거 핀으로 펄스 출력
	PORTC |= (1 << PC0);		// HIGH 값 출력
	_delay_us(10);				// 10 마이크로초 대기
	PORTC &= ~(1 << PC0);		// LOW 값 출력
	
	// 에코 핀이 HIGH가 될 때까지 대기
	TCNT0 = 0;
	while(!(PINC & 0x02))
		if(TCNT0 > 250) return 255;		// 장애물이 없는 경우
		
	// 에코 핀이 LOW가 될 때까지의 시간 측정
	TCNT0 = 0;		// 카운터를 0으로 초기화
	while(PINC & 0x02)
	{
		if(TCNT0 > 250)		// 장애물이 없는 경우
		{
			TCNT0 = 0;
			break;
		}
	}
	
	// 에코 핀의 펄스 폭을 마이크로초 단위로 계산
	double pulse_width = TCNT0 * PRESCALER * 1000000.0 / F_CPU;
	
	return pulse_width / 58;		// 센티미터 단위 거리 반환
}


int main(void)
{
    uint8_t distance;
	DDRC |= 0x01;		// 트리거 핀 출력으로 설정
	DDRC &= ~0xFD;		// 에코 핀 입력으로 설정 1111 1101
	
	UART_INIT();		// UART 통신 초기화
	Timer_init();		// 타이머 초기화
	
    while (1) 
    {
		distance = measure_distance();		// 거리 측정
		
		UART_printString("Distance(cm) = ");
		UART_print8bitNumber(distance);
		UART_transmit('\n');
		
		_delay_ms(1000);
    }
}