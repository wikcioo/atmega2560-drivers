#include "switch.h"

#include <avr/io.h>

void switch_init()
{
    // Reset the first 5 bits in DDRC register to set input mode for PC0 - PC5
    DDRC &= ~(0x1F);
    // Set the first 5 bits in PORTC reg to configure pull-ups for PC0 - PC5
    PORTC |= 0x1F;

    // Reset 3rd and 4th bits in DDRD register to set input mode for PD2 and PD3
    DDRD &= ~(0x3 << 2);
    // Set 3rd and 4th bits in PORTD register to configure pull-ups for PD2 and PD3
    PORTD |= (0x3 << 2);
}

b8 switch_get(b8 switch_no)
{
    // Inspect either PINC or PIND based on switch number
    b8 reg = (switch_no == SWITCH7 || switch_no == SWITCH8) ? PIND : PINC;

    // SWITCH7 and SWITCH8 correspond to PIND pins 2 and 3, respectively,
    // so their values need be offset accordingly
    switch_no = (switch_no == SWITCH7 || switch_no == SWITCH8) ? switch_no-4 : switch_no;

    // Inspect bit value of the corresponsing switch number
    b8 state = reg & (1 << switch_no);

    // Switches use pull-ups, which means that SWITCH_ON corresponds to grounded pin
    return state ? SWITCH_OFF : SWITCH_ON;
}
