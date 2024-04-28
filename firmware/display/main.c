#include <util/delay.h>
#include <bsp/display.h>

int main()
{
    display_init();

    while (1)
    {
        display_printint(420);
        _delay_ms(1000);
        display_printint(-69);
        _delay_ms(1000);
    }

    return 0;
}
