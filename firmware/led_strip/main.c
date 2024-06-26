#include <util/delay.h>

#include <bsp/led_array.h>

int main()
{
    led_array_init();

    while (1)
    {
        for (i8 i = LED_LAST; i >= 0; i--)
        {
            led_array_toggle(i);
            _delay_ms(50);
        }
    }

    return 0;
}
