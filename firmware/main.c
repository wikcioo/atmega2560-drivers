#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRA = 0xFF;
    PORTA = 0xFF;

    while (1)
    {
        PORTA ^= (1 << 0);
        _delay_ms(1000);
    }

    return 0;
}
