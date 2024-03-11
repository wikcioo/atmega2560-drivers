#include "led_array.h"

#include <avr/io.h>

void led_array_init()
{
    // Set all bits in DDRA to set output mode for PA0 - PA7
    DDRA = 0xFF;
    // Set all bits in PORTA to set the state of all leds to low
    PORTA = 0xFF;
}

void led_array_set(u8 led_no, u8 state)
{
    if (state == LED_HIGH)
    {
        // Reset the corresponding bit to ground, thus turning it on
        PORTA &= ~(1 << led_no);
    }
    else if (state == LED_LOW)
    {
        // Set the corresponding bit to high, thus turning it off
        PORTA |= (1 << led_no);
    }
}

void led_array_toggle(u8 led_no)
{
    // Toggle the corresponding bit
    PORTA ^= (1 << led_no);
}

void led_array_clear()
{
    // Set all bits to high, thus turning off all leds
    PORTA = 0xFF;
}
