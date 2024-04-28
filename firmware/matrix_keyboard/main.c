#include "bsp/display.h"
#include "bsp/matrix_keyboard.h"

#include <util/delay.h>

int main()
{
    display_init();
    matrix_keyboard_init();

    for (;;) {
        i8 key = matrix_keyboard_getkey();
        display_printint(key);
        _delay_ms(10);
    }

    return 0;
}
