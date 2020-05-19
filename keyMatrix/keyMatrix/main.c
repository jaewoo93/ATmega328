/*
 * keyMatrix.c
 *
 * Created: 2020-05-19 오전 9:30:41
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define ROWS	4
#define COLS	4

char keys[ROWS][COLS] = {		// 키 값 정의
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

uint8_t rowPins[ROWS] = {PB1, PB0, PD7, PD6};		// 행 연결 핀
uint8_t colPins[COLS] = {PD5, PD4, PD3, PD2};		// 열 연결 핀
	
void InitKeyMatrix(void)
{
	// 행(ROW) 연결 핀 설정 : 풀업 저항을 사용하는 입력
	DDRB &= ( ~(1 << PB1) & ~(1 << PB0) );		// 입력으로 설정. bit clear
	PORTB |= ( (1 << PB1) | (1 << PB0) );		// 풀업 사용
	DDRD &= ( ~(1 << PD7) & ~(1 << PD6) );		// 입력으로 설정. bit clear
	PORTD |= ( (1 << PD7) & (1 << PD6) );		// 풀업 사용
	
	// 열(COL) 연결 핀 설정
	// 하이 임피던스 상태로 만들기 위해 입력으로 설정
	DDRD &= 0b11000011;
}

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);		// 2배속 모드
	
	UBRR0H = 0x00;				// 통신 속도(Baud rate) 설정
	UBRR0L = 207;
	
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	UCSR0B |= _BV(RXEN0);					// 송수신 가능
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)) );			// 데이터 수신 대기
	return UDR0;
}

void UART_transmit(unsigned char data)
{
	while( !(UCSR0A & (1<<UDRE0)) );		// 송신 가능 대기
	UDR0 = data;							// 데이터 전송
}

void UART_transmit_string(char *str)
{
	while(*str)		// 문자열의 끝 검사
	{
		UART_transmit(*str);		// 문자 단위 전송
		str++;
	}
	
	UART_transmit('\n');		// 줄바꿈 문자 추가
}


int main(void)
{
    uint8_t keyPressed[ROWS][COLS];		// 키 상태 저장
	uint8_t row, col, count;
	
	UART_INIT();			// UART 통신 초기화
	InitKeyMatrix();		// 키 매트릭스 연결 초기화
	
    while (1) 
    {
		count = 0;			// 눌려진 버튼의 개수	
		
		for(col = 0; col < COLS; col++)
		{
			DDRD |= (1 << colPins[col]);		// 해당 COL을 출력으로 설정
			PORTD |= ~(1 << colPins[col]);		// 해당 COL에 LOW 출력
			_delay_ms(3);						// 출력 안정화
			
			// LOW의 입력 읽기 : LOW인 경우 버튼이 눌러진 상태
			keyPressed[0][col] = 1 - ((PINB & (1 << rowPins[0])) >> rowPins[0]);
			keyPressed[1][col] = 1 - ((PINB & (1 << rowPins[1])) >> rowPins[1]);
			keyPressed[2][col] = 1 - ((PINB & (1 << rowPins[2])) >> rowPins[2]);
			keyPressed[3][col] = 1 - ((PINB & (1 << rowPins[3])) >> rowPins[3]);
			
			for(row = 0; row < ROWS; row++)		// 눌러진 버튼 개수 읽기
			{
				count += keyPressed[row][col];
			}
			
			PORTD |= (1 << colPins[col]);		// 해당 COL에 HIGH 출력
			DDRD &= ~(1 << colPins[col]);		// 입력으로 (하이 임피던스 상태) 설정
		}
		
		if(count > 0)							// 버튼이 눌러진 경우만 UART로 출력
		{
			for(row = 0; row < ROWS; row++)
			{
				for(col = 0; col < COLS; col++)
				{
					if(keyPressed[row][col])
						UART_transmit(keys[row][col]);
					else
						UART_transmit('.');
					
					UART_transmit_string(" ");
				}
				UART_transmit('\n');
			}
			UART_transmit('\n');
		}
		
		_delay_ms(1000);		// 1초에 한 번만 검사
    }
}

