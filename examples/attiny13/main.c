/**
 * Copyright (c) 2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 *
 * This is ATtiny13 "Running Digits, Dots and LEDs" example using attiny-tm1637-library,
 * https://github.com/lpodkalicki/attiny-tm1637-library .
 */

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "tm1638.h"

static uint8_t scan_keys_demo(void);
static void digits_and_dots_demo(uint8_t n);
static void leds_demo(uint8_t n);

int
main(void)
{
	uint8_t n = 0, k = 0, keys;

	/* setup */
	TM1638_init(1/*enable*/, 3/*brighness*/);

	/* loop */
	while (1) {
		keys = scan_keys_demo();
		if ((k % 10) == 0) { 
			digits_and_dots_demo(n);
			if (!keys) {
				leds_demo(n);
			}
			n++;
		}
		k++;
		_delay_ms(30);
	}
}

uint8_t
scan_keys_demo(void)
{
	uint8_t i, keys;

	keys = TM1638_scan_keys();
	for (i = 0; i < 8; ++i) {
		TM1638_set_led((i + 1) % 8, !!(keys & (1 << i)));
	}

	return keys;
}

void
digits_and_dots_demo(uint8_t n)
{
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		TM1638_display_digit(i, (n + i) % 0x10, !!(n % 0x02));
	}
}

void
leds_demo(uint8_t n)
{
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		TM1638_set_led(i, !!((i + n) % 0x02));
	}
}
