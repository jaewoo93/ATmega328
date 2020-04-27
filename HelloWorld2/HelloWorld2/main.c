#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0xFF;
	
	while(1)
	{
		PORTD = 0x55;
		_delay_ms(1000);
		PORTD = 0xAA;
		_delay_ms(1000);
    }
	return 1;
}

