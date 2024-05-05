#include <bsp/switch.h>
#include <bsp/led_array.h>

int main()
{
    switch_init();
    led_array_init();

    while (1)
    {
        b8 s7 = switch_get(SWITCH7);
        b8 s8 = switch_get(SWITCH8);

        led_array_clear();

        if (s7 & s8)
        {
            led_array_set(LED8, LED_HIGH);
        }
        if (s7 | s8)
        {
            led_array_set(LED7, LED_HIGH);
        }
        if (s7 ^ s8)
        {
            led_array_set(LED6, LED_HIGH);
        }
        if (!(s7 & s8))
        {
            led_array_set(LED5, LED_HIGH);
        }
        if (!(s7 | s8))
        {
            led_array_set(LED4, LED_HIGH);
        }
        if (!(s7 ^ s8))
        {
            led_array_set(LED3, LED_HIGH);
        }
    }

    return 0;
}
