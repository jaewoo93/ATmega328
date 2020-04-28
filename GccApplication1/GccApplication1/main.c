/*
 * GccApplication1.c
 *
 * Created: 2020-04-28 오후 2:17:07
 * Author : user
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0x20;
	
	while(1)
	{
		PORTD = 0x20;
		_delay_ms(1000);
		PORTD = 0x00;
		_delay_ms(1000);
	}
	return 1;
}

