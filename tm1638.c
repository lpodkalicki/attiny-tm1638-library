/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * 
 * This is ATtiny13/25/45/85 library for TM1638 chip - LED driver controller with key-scan interface.
 *
 * Features:
 * - display digits & dots
 * - display raw segments
 * - display LEDs
 * - display on/off
 * - brightness control
 * - scan keys
 *
 * References:
 * - library: https://github.com/lpodkalicki/attiny-tm1638-library
 * - documentation: https://github.com/lpodkalicki/attiny-tm1638-library/README.md
 * - TM1638 datasheet: https://github.com/lpodkalicki/attiny-tm1638-library/blob/master/docs/tm1638.pdf
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "tm1638.h"

#define	TM1638_DIO_HIGH()	(PORTB |= _BV(TM1638_DIO_PIN))
#define	TM1638_DIO_LOW()	(PORTB &= ~_BV(TM1638_DIO_PIN))
#define	TM1638_DIO_OUTPUT()	(DDRB |= _BV(TM1638_DIO_PIN))
#define	TM1638_DIO_INPUT()	(DDRB &= ~_BV(TM1638_DIO_PIN))
#define	TM1638_DIO_READ()	(((PINB & _BV(TM1638_DIO_PIN)) > 0) ? 1 : 0)
#define	TM1638_CLK_HIGH()	(PORTB |= _BV(TM1638_CLK_PIN))
#define	TM1638_CLK_LOW()	(PORTB &= ~_BV(TM1638_CLK_PIN))
#define	TM1638_STB_HIGH()	(PORTB |= _BV(TM1638_STB_PIN))
#define	TM1638_STB_LOW()	(PORTB &= ~_BV(TM1638_STB_PIN))


static void TM1638_send_config(const uint8_t enable, const uint8_t brightness);
static void TM1638_send_command(uint8_t value);
static void TM1638_write_byte(uint8_t value);
static uint8_t TM1638_read_byte(void);
static void TM1638_send_data(uint8_t address, uint8_t data);

static uint8_t _config = TM1638_SET_DISPLAY_ON | TM1638_MAX_BRIGHTNESS;
PROGMEM const uint8_t _digit2segments[10] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F  // 9
};

void
TM1638_init(const uint8_t enable, const uint8_t brightness)
{
	uint8_t i;

	DDRB |= (_BV(TM1638_DIO_PIN) | _BV(TM1638_CLK_PIN) | _BV(TM1638_STB_PIN));
	
	TM1638_STB_HIGH();
	TM1638_CLK_HIGH();

	TM1638_STB_LOW();
	TM1638_write_byte(TM1638_CMD_SET_ADDR);
	for (i = 0; i < 16; i++) {
		TM1638_write_byte(0x00);
	}
	TM1638_STB_HIGH();

	TM1638_send_config(enable, brightness);
}

void
TM1638_enable(const uint8_t value)
{

	TM1638_send_config(value, _config & TM1638_MAX_BRIGHTNESS);
}

void
TM1638_set_brightness(const uint8_t brightness)
{

	TM1638_send_config(_config & TM1638_SET_DISPLAY_ON,
		brightness & TM1638_MAX_BRIGHTNESS);
}

void
TM1638_display_segments(const uint8_t position, const uint8_t segments)
{

	TM1638_send_data(position << 1, segments);
}

void
TM1638_clear_segments(void)
{
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		TM1638_display_segments(i, 0x00);
	}
}

void
TM1638_display_digit(const uint8_t position, const uint8_t digit, const uint8_t dot)
{

	TM1638_display_segments(position, 
		(digit < 10 ? pgm_read_byte_near((uint8_t *)&_digit2segments + digit) : 0x00) |
		(dot ? 0b10000000 : 0));
}

void
TM1638_set_led(const uint8_t position, const uint8_t value)
{

	TM1638_send_data((position << 1) - 1, !!value);
}

void
TM1638_clear_leds(void)
{
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		TM1638_set_led(i, 0x00);
	}
}

uint8_t
TM1638_scan_keys(void)
{
	uint8_t i, keys = 0;

  	TM1638_STB_LOW();
  	TM1638_write_byte(TM1638_CMD_SET_DATA|TM1638_SET_DATA_READ);
	for (i = 0; i < 4; i++) {
		keys |= TM1638_read_byte() << i;
	}
	TM1638_STB_HIGH();

  	return keys;
}

void
TM1638_send_config(const uint8_t enable, const uint8_t brightness)
{
	
	_config = (enable ? TM1638_SET_DISPLAY_ON : TM1638_SET_DISPLAY_OFF) |
		(brightness > TM1638_MAX_BRIGHTNESS ? TM1638_MAX_BRIGHTNESS : brightness);

	TM1638_send_command(TM1638_CMD_SET_DATA);
	TM1638_send_command(TM1638_CMD_SET_DSIPLAY | _config);
}

void
TM1638_send_command(uint8_t value)
{
	TM1638_STB_LOW();
	TM1638_write_byte(value);
	TM1638_STB_HIGH();
}

uint8_t
TM1638_read_byte(void)
{
	uint8_t i, result = 0;

	TM1638_DIO_INPUT();
	TM1638_DIO_HIGH();

	for (i = 0; i < 8; i++) {
		TM1638_CLK_LOW();
		if (TM1638_DIO_READ()) {
			result |= 0x80;
		}
		result >>= 1;
		TM1638_CLK_HIGH();		
	}

	TM1638_DIO_OUTPUT();
	TM1638_DIO_LOW();
	
	return result;
}

void
TM1638_write_byte(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 8; ++i, value >>= 1) {
		TM1638_CLK_LOW();
		_delay_us(TM1638_DELAY_US);
		if (value & 0x01) {
			TM1638_DIO_HIGH();
		} else {
			TM1638_DIO_LOW();
		}
		TM1638_CLK_HIGH();
		_delay_us(TM1638_DELAY_US);
	}
}

void
TM1638_send_data(uint8_t address, uint8_t data)
{
	TM1638_send_command(TM1638_CMD_SET_DATA | TM1638_SET_DATA_F_ADDR);
  	TM1638_STB_LOW();
  	TM1638_write_byte(TM1638_CMD_SET_ADDR | address);
  	TM1638_write_byte(data);
	TM1638_STB_HIGH();
}

