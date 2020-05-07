/*
 * DCmotor.c
 *
 * Created: 2020-05-07 오후 6:00:34
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= 0x0E;		// 모터 제어를 위한 핀들을 출력으로 설정. 0000 1110
    
	// 타이머/카운터 2번을 고속 PWM 모드로 설정
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2A |= (1 << COM2A1);		// 비반전 모드
	TCCR2B |= (1 << CS22);		// 분주비 64
	
	PORTB |= 0x04;		// 초기 상태는 비활성화 상태
	
	while (1) 
    {
		PORTB &= ~0x04;		//모터1 활성화. 0000 0100
		
		PORTB |= 0x02;		// 정방향 회전
		for(int i = 0; i < 256; i++)		// 속도 조절을 위한 PWM 신호 출력
		{
			OCR2A = i;
			_delay_ms(25);
		}
		OCR2A = 0;		// 모터 정지
		
		_delay_ms(1000);
		
		PORTB &= ~0x02;		// 역방향 회전
		for(int i = 0; i < 256; i++)		// 속도 조절을 위한 PWM 신호 출력
		{
			OCR2A = i;
			_delay_ms(25);
		}
		OCR2A = 0;		// 모터 정지
		
		PORTB |= 0x04;		// 모터1 비활성화
		
		while(1);
    }
}