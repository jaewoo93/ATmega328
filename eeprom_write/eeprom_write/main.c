/*
 * eeprom_write.c
 *
 * Created: 2020-05-08 오후 4:59:16
 * Author : user
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>

void UART_INIT(void)
{
	UCSR0A |= _BV(U2X0);						// 2배속 모드
	
	UBRR0H = 0x00;								// 통신 속도(보율) 설정
	UBRR0L = 207;								// 보율 = 9600
	
	// 비동기, 8비트 데이터, 패리티 없음, 1비트 정지 비트 모드
	UCSR0C |= 0x06;
	
	UCSR0B |= _BV(RXEN0);						// 송수신 가능
	UCSR0B |= _BV(TXEN0);
}

unsigned char UART_receive(void)				// 1바이트 수신
{
	while( !(UCSR0A & (1<<RXC0)) );				// 데이터 수신 대기
	return UDR0;
}

void UART_transmit(unsigned char data)			// 1바이트 송신
{
	while( !(UCSR0A & (1<<UDRE0)) );			// 송신 가능 대기
	UDR0 = data;
}

void UART_printString(char *str)				// 문자열 송신
{
	for(int i=0; str[i]; i++)					// '\0' 문자를 만날 때까지 반복
	UART_transmit(str[i]);					// 바이트 단위 출력
}

void UART_print8bitNumber(uint8_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

void UART_print16bitNumber(uint16_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[6] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

void UART_print32bitNumber(uint32_t no)			// 숫자를 문자열로 변환하여 송신, 8비트
{
	char numString[11] = "0";
	int i, index = 0;
	
	if(no > 0)									// 문자열 변환
	{
		for(i=0; no!=0; i++)
		{
			numString[i] = no % 10 + '0';
			no = no / 10;
		}
		numString[i] ='\0';
		index = i-1;
	}
	for(i=index; i>=0; i--)						 // 변환된 문자열 출력
	UART_transmit(numString[i]);
}

int main(void)
{
    // EEPROM에 쓸 데이터
	uint8_t dataByte = 1;
	uint16_t dataWord = 2;
	uint32_t dataDWord = 3;
	char dataString[] = "ABCDE";
	
	// EEPROM의 주소
	int addressByte = 0, addressWord = 10, addressDWord = 20, addressString = 30;
	
	UART_INIT();		// UART 통신 초기화
	
	// 데이터 쓰기
	eeprom_update_byte( (uint8_t *)addressByte, dataByte * 10);
	eeprom_update_word( (uint16_t *)addressWord, dataWord * 10);
	eeprom_update_dword( (uint32_t *)addressDWord, dataDWord * 10);
	eeprom_update_block( (void *)dataString, (void *)addressString, 5);
	
	// 데이터 읽기
	dataByte = eeprom_read_byte( (uint8_t *)addressByte );
	dataWord = eeprom_read_word( (uint16_t *)addressWord );
	dataDWord = eeprom_read_dword( (uint32_t *)addressDWord );
	eeprom_read_block( (void *)dataString, (void *)addressString, 5);
	
	// UART 통신으로 데이터 전송
	UART_printString("Byte      : ");		// 1바이트 데이터
	UART_print8bitNumber(dataByte);
	UART_printString("\n");
	UART_printString("Word      : ");		// 2바이트 데이터
	UART_print16bitNumber(dataWord);
	UART_printString("\n");
	UART_printString("Double Word      : ");		// 4바이트 데이터
	UART_print32bitNumber(dataDWord);
	UART_printString("\n");
	UART_printString("String      : ");		// 문자열 데이터
	UART_printString(dataString);
	UART_printString("\n");
	
    while(1);
	
	return 0;
}