#include "TEXT_LCD.h"

void LCD_pulse_enable(void)				// �ϰ� �������� ����
{
	PORT_CONTROL |= (1 << E_PIN);		// E�� HIGH��
	_delay_us(1);
	PORT_CONTROL &= ~(1 << E_PIN);		// E�� LOW��
	_delay_ms(1);
}

void LCD_write_data(uint8_t data)
{
	PORT_CONTROL |= (1 << RS_PIN);		// ���� ��¿��� RS�� 1
	PORT_DATA = data;					// ����� ���� ������
	LCD_pulse_enable();					// ���� ���
}

void LCD_write_command(uint8_t command)
{
	PORT_CONTROL &= ~(1 << RS_PIN);		// ��ɾ� ���࿡�� RS�� 0
	PORT_DATA = command;				// ������ �ɿ� ��ɾ� ����
	LCD_pulse_enable();					// ��ɾ� ����
}

void LCD_clear(void)
{
	LCD_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(2);
}

void LCD_init(void)
{
	_delay_ms(50);
	
	// ���� ���� ������� ����
	DDR_DATA = 0xFF;
	DDR_CONTROL |= (1 << RS_PIN) | (1 << E_PIN);
	
	LCD_write_command(COMMAND_8_BIT_MODE);		// 8��Ʈ ���
	
	// ȭ�� on, Ŀ�� off, Ŀ�� ������ off
	uint8_t command = 0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT);
	LCD_write_command(command);
	
	LCD_clear();		// ȭ�� ����
	
	// Entry Mode Set. ��� �� �ּ� ����, ȭ�� �̵� ����
	LCD_write_command(0x06);
}

void LCD_write_string(char *string)
{
	uint8_t i;
	for(i = 0; string[i]; i++)			// ���� ���ڸ� ���� ������
	LCD_write_data(string[i]);		// ���� ���� ���
}

void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %= 16;		// [0 15]
	row %= 2;		// [0 1]
	
	// ù° ���� ���� �ּҴ� 0x00, ��° ���� ���� �ּҴ� 0x40
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	
	LCD_write_command(command);		// Ŀ�� �̵�
}