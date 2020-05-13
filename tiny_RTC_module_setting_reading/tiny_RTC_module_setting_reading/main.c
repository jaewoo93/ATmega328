/*
 * tiny_RTC_module_setting_reading.c
 *
 * Created: 2020-05-13 오전 11:11:20
 * Author : user
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#include "I2C.h"

uint8_t bcd_to_decimal(uint8_t bcd)				// BCD 형식 -> 이진수 형식
{
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t decimal_to_bcd(uint8_t decimal)				// 이진수 형식 -> BCD 형식
{
	return ( ((decimal / 10) << 4) | (decimal % 10) );
}

void write_message(uint8_t no, char *message)		// RTC 모듈 데이터 출력
{
	UART_printString(" ");
	UART_print8bitNumber(no);
	UART_printString(message);
}

int main(void)
{
    uint8_t i;
	
	I2C_init();		// I2C 초기화
	UART_INIT();	// UART 초기화
	
	uint8_t address = 0x68;		// RTC 모듈의 I2C 주소
	
	// 초, 분, 시, 요일, 일, 월, 년
	// 2014년 9월 1일 월요일 12시 34분 56초
	uint8_t data[] = {56, 34, 12, 2, 1, 9, 14};
		
	// RTC 모듈에 시간 설정
	I2C_start();					// I2C 시작
	I2C_transmit(address << 1);		// I2C 주소 전송, 쓰기 모드
	I2C_transmit(0);				// RTC에 데이터를 기록할 메모리 시작 주소 전송
	
	UART_printString("* Setting RTC module...\n");
	for(i = 0; i < 7; i++)
	{
		UART_printString(" ");
		UART_print8bitNumber(i);
		UART_printString("th byte write...\n");
		
		I2C_transmit(decimal_to_bcd(data[i]));
	}
	I2C_stop();		// I2C 정지
	
	_delay_ms(2000);		// 2초 대기 후 현재 시간
	
	I2C_start();
	I2C_transmit(address << 1);
	I2C_transmit(0);
	I2C_stop();
	
	I2C_start();
	I2C_transmit( (address << 1) + 1);
	
	UART_printString("* Time/Date Retrieval...\n");
	
	write_message(bcd_to_decimal(I2C_receive()), " second\n");
	write_message(bcd_to_decimal(I2C_receive()), " minute\n");
	write_message(bcd_to_decimal(I2C_receive()), " hour\n");
	write_message(bcd_to_decimal(I2C_receive()), " day of week\n");
	write_message(bcd_to_decimal(I2C_receive()), " day\n");
	write_message(bcd_to_decimal(I2C_receive()), " month\n");
	write_message(bcd_to_decimal(I2C_receive()), " year\n");
	
	I2C_stop();
	
	while(1);
}

