#pragma once

#include "defines.h"

// #LED_NUMBERS
#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 4
#define LED6 5
#define LED7 6
#define LED8 7
#define LED_LAST LED8

// #LED_STATES
#define LED_LOW  0
#define LED_HIGH 1

/*
 * Initialize led array
 */
void led_array_init();

/*
 * Set the state of an led to low or high
 * @param led_no Led number. See #LED_NUMBERS for possible values
 * @param state Led state. See #LED_STATES for possible values
 */
void led_array_set(u8 led_no, u8 state);

/*
 * Toggle the state of an led
 * @param led_no Led number. See #LED_NUMBERS for possible values
 */
void led_array_toggle(u8 led_no);
