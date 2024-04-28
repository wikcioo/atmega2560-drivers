#include <util/delay.h>

#include <hal/switch.h>
#include <hal/led_array.h>

void on_switch_pressed(b8 increment)
{
    static i16 number = 0;

    if (increment && number < 255)
        number++;
    else if (!increment && number > 0)
        number--;

    for (u8 i = 0; i < 8; i++)
    {
        if (number & (1 << i))
            led_array_set(i, LED_HIGH);
        else
            led_array_set(i, LED_LOW);
    }

    _delay_ms(50);
}

int main()
{
    led_array_init();
    switch_init(0, 0, on_switch_pressed);

    while (1)
        ;

    return 0;
}
