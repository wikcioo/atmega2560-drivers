#include "led_array.h"

#include <avr/io.h>

void led_array_init()
{
    DDRA = 0xFF;
    PORTA = 0xFF;
}

void led_array_set(u8 led_no, u8 state)
{
    if (state == LED_HIGH)
    {
        PORTA &= ~(1 << led_no);
    }
    else if (state == LED_LOW)
    {
        PORTA |= (1 << led_no);
    }
}

void led_array_toggle(u8 led_no)
{
    PORTA ^= (1 << led_no);
}
