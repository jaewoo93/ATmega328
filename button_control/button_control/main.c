/*
 * button_control.c
 *
 * Created: 2020-04-29 오전 11:37:35
 * Author : user
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

void INIT_PORT(void)
{
	DDRB = 0x20;		// PB5 출력으로 설정
	PORTB = 0x00;		// LED 꺼진 상태에서 시작
	
	DDRD = 0x00;		// 버튼 입력 설정
	PORTD = 0x04;		// 0000 0100. PD2 풀업 저항 사용
}


int main(void)
{
    INIT_PORT();		// 포트 설정
    while (1) 
    {
		// 풀업 저항을 사용했으므로 눌렀을 때 LOW 동작
		if(PIND & 0x04)		PORTB = 0x00;		// LED 끄기
		else				PORTB = 0x20;		// LED 켜기
    }
	
	return 1;
}

