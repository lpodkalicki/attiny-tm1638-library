# TM1638 Library
This is tinyAVR (ATtiny13, ATtiny25, ATtiny45, ATtiny85, and other) library for LED controler modules based on TM1638 chip. 

Modules based on TM1638 provide three signal connections (CLK, STB and DIO) and two power connections (VCC and GND). Signal pins can be connected to any pair of digital pins of the AVR chip. Signal pins configuration is defined at the top of library header file, where it can be modifed.

## Key Features
This library has the following features:
 * display digits & dots,
 * display raw segments,
 * display LEDs,
 * display on/off,
 * brightness control,
 * scan keys,
 * software I2C.

The communication part has been inspired by projects: 
 * https://github.com/rjbatista/tm1638-library/, 
 * https://github.com/lpodkalicki/attiny-tm1637-library.

## Example Code
This example code demonstrates basic usage of the library:

```c
#include <stdint.h>
#include <util/delay.h>
#include "tm1638.h"

int
main(void)
{
	uint8_t j, i = 0;

	/* setup */
	TM1638_init(1/*enable*/, 7/*brighness*/);

	/* loop */
	while (1) {
		for (j = 0; j < 8; ++j) {
			TM1638_display_digit(j/*position*/,
				(i + j) % 0x10/*digit*/,
				!!(i % 0x02)/*dot*/);
		}
		_delay_ms(100);
		i++;
	}
}
```

## API Documentation (tm1638.h)

```c
/**
 * Initialize TM1638 LED controller.
 * Clock pin (TM1638_CLK_PIN), strobe pin (TM1638_STB_PIN)
 * and data pin (TM1638_DIO_PIN) are defined at the top of this file.
 */
void TM1638_init(const uint8_t enable, const uint8_t brightness);

/**
 * Set display brightness.
 * Min brightness: 0
 * Max brightness: 7
 */
void TM1638_set_brightness(const uint8_t brightness);

/**
 * Turn display on/off.
 * value: 1 - on, 0 - off
 */
void TM1638_enable(const uint8_t value);

/**
 * Display raw segments at position (0x00..0x07)
 * 
 *      bits:
 *        -- 0 -- 
 *       |       |
 *       5       1
 *       |       |
 *        -- 6 -- 
 *       |       |
 *       4       2
 *       |       |
 *        -- 3 -- *7
 *
 * Example segment configurations:
 * - for character 'H', segments=0b01110110
 * - for character '-', segments=0b01000000
 * - etc.
 */
void TM1638_display_segments(const uint8_t position, const uint8_t segments);

/**
 * Clear all display segments (including dots).
 */
void TM1638_clear_segments(void);

/**
 * Display digit ('0'..'9') with optional dot at position (0x00..0x07)
 */ 
void TM1638_display_digit(const uint8_t position, const uint8_t digit, const uint8_t dot);

/**
 * Light LED at position (0x00..0x07)
 * value: 1 - on, 0 - off
 */ 
void TM1638_set_led(const uint8_t position, const uint8_t value);

/**
 * Clear all leds.
 */
void TM1638_clear_leds(void);

/**
 * Scan keys.
 */
uint8_t TM1638_scan_keys(void);

```
