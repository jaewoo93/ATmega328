/*
 * fastPWM_mode.c
 *
 * Created: 2020-05-05 오후 8:03:13
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	// 파형 출력 핀인 PB3 핀(OC2A 핀. 아두이노 11번)을 출력으로 설정
	DDRB |= (1 << PB3);
	
	// 타이머/카운터 2번을 고속 PWM 모드로 설정
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2A |= (1 << COM2A1);		// 비반전 모드
	TCCR2B |= (1 << CS22);		// 분주비 64
	
    while (1) 
    {
		for(int i = 0; i < 256; i++)
		{
			OCR2A = i;
			_delay_ms(10);
		}
    }
}

