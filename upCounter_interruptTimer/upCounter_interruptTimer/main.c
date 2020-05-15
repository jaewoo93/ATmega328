/*
 * upCounter_interruptTimer.c
 *
 * Created: 2020-05-14 오전 11:35:43
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int interrupt_count = 0;		// 인터럽트 발생 횟수

ISR(TIMER0_OVF_vect)		// 타이머/카운터 0번 오버플로 인터럽트
{
	interrupt_count++;
}

int main(void)
{
    uint8_t numbers[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	int count = 0;
	DDRD = 0xFF;		// 제어핀 8개 출력으로 설정
	PORTD = numbers[0];		// 0에서 시작
	
	// 타이머/카운터 0번 인터럽트 설정
	TCCR0B |= (1 << CS02) | (1 << CS00);		// 분주비를 1024로 설정. clk / 1024
	TIMSK0 |= (1 << TOIE0);		// 오버플로 인터럽트 허용
	sei();		// 전역적으로 인터럽트 허용
	
	
	
    while (1) 
    {
		if(interrupt_count >= 64)		// 1초 경과
		{
			interrupt_count = 0;		// 인터럽트 발생 횟수 초기화
			
			count = (count + 1) & 10;		// 표시할 숫자
			PORTD = numbers[count];		// 숫자 데이터 출력
		}
    }
	
	return 0;
}

