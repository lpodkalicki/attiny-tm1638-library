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

#ifndef	_ATTINY_TM1638_H_
#define	_ATTINY_TM1638_H_

#include <stdint.h>

// Main Settings
#define	TM1638_DIO_PIN			PB0
#define	TM1638_CLK_PIN			PB1
#define	TM1638_STB_PIN			PB2
#define	TM1638_DELAY_US			(5)
#define	TM1638_MAX_BRIGHTNESS		0x07

// TM1638 commands
#define	TM1638_CMD_SET_DATA		0x40
#define	TM1638_CMD_SET_ADDR		0xC0
#define	TM1638_CMD_SET_DSIPLAY		0x80

// TM1638 data settings (use bitwise OR to contruct complete command)
#define	TM1638_SET_DATA_WRITE		0x00 // write data to the display register
#define	TM1638_SET_DATA_READ		0x02 // read the key scan data
#define	TM1638_SET_DATA_A_ADDR		0x00 // automatic address increment
#define	TM1638_SET_DATA_F_ADDR		0x04 // fixed address
#define	TM1638_SET_DATA_M_NORM		0x00 // normal mode
#define	TM1638_SET_DATA_M_TEST		0x10 // test mode

// TM1638 display control command set (use bitwise OR to consruct complete command)
#define	TM1638_SET_DISPLAY_OFF		0x00 // off
#define	TM1638_SET_DISPLAY_ON		0x08 // on


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

#endif	/* !_ATTINY_TM1638_H_ */

