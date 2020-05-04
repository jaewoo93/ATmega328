/*
 * compare_match_interrupt_blink3.c
 *
 * Created: 2020-05-04 오후 8:12:13
 * Author : user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

int state = 0;		// LED 점멸 상태

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;		// 수동 설정. 자동으로 0으로 변하지 않는다
	
	state = !state;		// LED 상태 반전
	if(state)	PORTB = 0xFF;		// LED 켜기
	else		PORTB = 0x00;		// LED 끄기
}

int main(void)
{
	DDRB = 0x20;		// PB5 핀을 출력으로 설정
	PORTB = 0x00;		// LED는 끈 상태에서 시작
	
	TCCR1B |= (1 << CS12) | (1 << CS10);		// 분주비를 1024로 설정
	
	OCR1A = 0x2000;		// 비교일치 기준값
	
	TIMSK1 |= (1 << OCIE1A);		// 비교일치 인터럽트 허용
	sei();		// 전역적으로 인터럽트 허용
    
    while (1)
	{
	}
}




