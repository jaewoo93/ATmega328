#ifndef _TEXT_LCD_H_
#define _TEXT_LCD_H_

#ifndef F_CPU
#define F_CPU	16000000
#endif

#include <avr/io.h>
#include <util/delay.h>

#define PORT_DATA						PORTD		// ������ �� ���� ��Ʈ
#define PORT_CONTROL					PORTB		// ���� �� ���� ��Ʈ
#define DDR_DATA						DDRD		// ������ ���� ������ ����
#define DDR_CONTROL						DDRB		// ���� ���� ������ ����

#define RS_PIN							0			// RS ���� ���� ��Ʈ ��ȣ
#define E_PIN							1			// E ����

#define COMMAND_CLEAR_DISPLAY			0x01
#define COMMAND_8_BIT_MODE				0x38		// 8��Ʈ, 2����, 5x8 ��Ʈ
#define COMMAND_4_BIT_MODE				0x28		// 4��Ʈ, 2����, 5x8 ��Ʈ

#define COMMAND_DISPLAY_ON_OFF_BIT		2
#define COMMAND_CURSOR_ON_OFF_BIT		1
#define COMMAND_BLINK_ON_OFF_BIT		0

extern uint8_t MODE;

void LCD_pulse_enable(void);							// E�� HIGH -> LOW�� ��ȭ
void LCD_write_data(uint8_t data);						// ������ �������� ���� ���
void LCD_write_command(uint8_t command);				// ��ɾ� ����
void LCD_clear(void);									// ȭ���� ���� ���ڷ� ����
void LCD_init(void);									// �ؽ�Ʈ LCD �ʱ�ȭ
void LCD_write_string(char *string);					// ���ڿ� ���
void LCD_goto_XY(uint8_t row, uint8_t col);				// ������ ��ġ�� Ŀ�� �̵�

#endif