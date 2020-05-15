/*
 * FND_4ea_2.c
 *
 * Created: 2020-05-15 오후 5:09:45
 * Author : user
 *
 * 4자리 7세그먼트 표시장치에 4자리 숫자 나타내기 1
 * -> 대기시간이 0.1초로 길어 잔상효과 X
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define SEGMENT_DELAY	5


int main(void)
{
	uint8_t numbers[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
	int count = 1000;		// 현재 표시할 숫자 (0 ~ 9)
	int thousands, hundreds, tens, ones;
	
	DDRD = 0xFF;		// 세그먼트 제어핀 8개를 출력으로 설정
	DDRB = 0x0F;		// 자릿수 선택핀 4개를 출력으로 설정
	
	while (1)
	{
		thousands = count / 1000;			// 천의 자리
		hundreds = count / 100 % 10;		// 백의 자리
		tens = count / 10 % 10;				// 십의 자리
		ones = count % 10;					// 일의 자리
		
		PORTB = 0x0E;		// 천의 자리 출력. 0000 1110
		PORTD = numbers[thousands];
		_delay_ms(SEGMENT_DELAY);
		
		PORTB = 0x0D;		// 백의 자리 출력. 0000 1101
		PORTD = numbers[hundreds];
		_delay_ms(SEGMENT_DELAY);
		
		PORTB = 0x0B;		// 십의 자리 출력. 0000 1011
		PORTD = numbers[tens];
		_delay_ms(SEGMENT_DELAY);
		
		PORTB = 0x07;		// 일의 자리 출력. 0000 0111
		PORTD = numbers[ones];
		_delay_ms(SEGMENT_DELAY);
		
		_delay_ms(100);
		
		count++;
		if(count == 9999)	count = 1000;
	}
	
	return 1;
}