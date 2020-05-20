/*
 * temperature_sensor_LM35.c
 *
 * Created: 2020-05-20 오후 4:57:06
 * Author : user
 */ 

#define  F_CPU 16000000L
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

void ADC_INIT(unsigned char channel)
{
	ADMUX |= 0x40; 			// AVCC를 기준 전압으로 선택
	
	ADCSRA |= 0x07;			// 분주비 설정
	ADCSRA |= (1 << ADEN);		// ADC 활성화
	ADCSRA |= (1 << ADATE);		// 자동 트리거 모드

	ADMUX |= ((ADMUX & 0xE0) | channel);	// 채널 선택
	ADCSRA |= (1 << ADSC);		// 변환 시작
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));	// 변환 종료 대기
	
	return ADC;				// 10비트 값을 반환
}


int main(void)
{
    int read;
	float input_voltage, temperature;
	
	UART_INIT();		// UART 통신 초기화
	ADC_INIT(1);		// AD 변환기 초기화
	
    while (1) 
    {
		// 온도 센서의 출력 전압을 ADC를 거쳐 읽는다.
		read = read_ADC();
		
		// 0에서 1023 사이의 값을 0V에서 5V 사이 값으로 변환한다.
		input_voltage = read * 5.0 / 1023.0;
		
		// 10mV에 1℃이므로 100을 곱해서 현재 온도를 얻는다.
		temperature = input_voltage * 100.0;
		
		UART_print16bitNumber((int)temperature);		// 정수값으로 출력
		UART_transmit('\n');						// 줄바꿈
		
		_delay_ms(1000);							// 1초에 한 번 출력
    }
}

