/*
 * servo_motor_with_dial.c
 *
 * Created: 2020-05-06 오후 8:50:34
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define ROTATION_DELAY	10		// 10ms 대기
// 모터 종류에 따라 상이. datasheet 확인 요망!
#define PULSE_MIN		544	// 최소 펄스 길이
#define PULSE_MAX		2400	// 최대 펄스 길이

void INIT_TIMER1(void)
{
	// Fast PWM 모드, TOP = ICR1
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	TCCR1B |= (1 << CS11);		// 분주율 8. 2MHz
	
	ICR1 = 41942;		// 20ms 주기
	
	TCCR1A |= (1 << COM1A1);		// 비반전 모드
	
	// OC1A (PB1, 아두이노 9번) 핀을 출력으로 설정
	DDRB |= (1 << PB1);
}

void ADC_INIT(unsigned char channel)
{
	ADMUX |= 0x40;		// AVCC를 기준 전압으로 선택
	
	ADCSRA |= 0x07;		 // 분주비 설정
	ADCSRA |= (1 << ADEN);		// ADC 활성화
	ADCSRA |= (1 << ADATE);		// 자동 트리거 모드
	
	ADMUX |= ((ADMUX & 0xE0) | channel);		// 채널 선택
	ADCSRA |= (1 << ADSC);		// 변환 시작
}

int read_ADC(void)
{
	while(!(ADCSRA & (1 << ADIF)));		// 변환 종료 대기
	
	return ADC;		// 10비트 값을 변환
}

int main(void)
{
    int read;
	
	ADC_INIT(0);		// AD 변환기 초기화
	INIT_TIMER1();
	
    while (1) 
    {
		read = read_ADC();		// 가변저항 값 읽기
		OCR1A = PULSE_MIN + (int)(3.48 * read);
		
		_delay_ms(ROTATION_DELAY);
    }
}