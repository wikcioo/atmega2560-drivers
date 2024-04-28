#include <util/delay.h>

#include <bsp/temperature.h>
#include <hal/led_array.h>
#include <bsp/display.h>

#define TEMPERATURE_LOW_BAR 18
#define TEMPERATURE_SAMPLING_PERIOD_MS 1000

int main()
{
    temperature_init(TEMPERATURE_SAMPLING_PERIOD_MS);
    display_init();
    led_array_init();

    while (1)
    {
        i16 temperature = temperature_get();
        led_array_bar(temperature - TEMPERATURE_LOW_BAR);
        display_printint(temperature);
        _delay_ms(1000);
    }

    return 0;
}
