/*
 * HelloWorld.c
 *
 * Created: 2020-04-25 오후 5:30:53
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB = 0x20;
	
    while (1) 
    {
		PORTB = 0x00;		// 끄기
		_delay_ms(1000);
		PORTB = 0x20;		// 켜기
		_delay_ms(1000);
    }
	
	return 1;
}

