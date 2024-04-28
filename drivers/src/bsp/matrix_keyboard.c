#include "matrix_keyboard.h"

#include <avr/io.h>

typedef struct matrix_desc
{
    volatile u8 *ddr;
    volatile u8 *port;
    volatile u8 *pin;
    u8 bit;
} matrix_desc_t;

/*
 *   rows: (output)
 *       Option 1       Option 2
 *       OC0A -> PB7    T0   -> PD7
 *       OC1A -> PB5    OC0B -> PG5
 *       OC2A -> PB4    OC1B -> PB6
 *       OC3A -> PE3A   OC2B -> PH6
 *
 *   cols: (input)
 *       Option 1       Option 2
 *       OC3C -> PE5    OB3B -> PE4
 *       OC4A -> PH3    ICP4 -> PL0
 *       OC4C -> PH5    OC4B -> PH4
 *       ICP5 -> PL1    T5   -> PL2
**/

/*
 *   Option 1: even pins
 *
 *   static const matrix_desc_t matrix[8] = {
 *       // cols
 *       { &DDRB, &PORTB, &PINB, PB7 },
 *       { &DDRB, &PORTB, &PINB, PB5 },
 *       { &DDRB, &PORTB, &PINB, PB4 },
 *       { &DDRE, &PORTE, &PINE, PE3 },
 *
 *       // rows
 *       { &DDRE, &PORTE, &PINE, PE5 },
 *       { &DDRH, &PORTH, &PINH, PH3 },
 *       { &DDRH, &PORTH, &PINH, PH5 },
 *       { &DDRL, &PORTL, &PINL, PL1 }
 *   };
**/

// Option 2: odd pins
static const matrix_desc_t matrix[8] = {
    /* cols */
    { &DDRD, &PORTD, &PIND, PD7 },
    { &DDRG, &PORTG, &PING, PG5 },
    { &DDRB, &PORTB, &PINB, PB6 },
    { &DDRH, &PORTH, &PINH, PH6 },

    /* rows */
    { &DDRE, &PORTE, &PINE, PE4 },
    { &DDRL, &PORTL, &PINL, PL0 },
    { &DDRH, &PORTH, &PINH, PH4 },
    { &DDRL, &PORTL, &PINL, PL2 }
};

static const i8 matrix_map[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

void matrix_keyboard_init(void)
{
    for (u8 i = 0; i < 4; i++) {
        // set as output
        *(matrix[i].ddr)  |= _BV(matrix[i].bit);
        // set as high state by default
        *(matrix[i].port) |= _BV(matrix[i].bit);
    }

    for (u8 i = 4; i < 8; i++) {
        // set as input
        *(matrix[i].ddr)  &= ~_BV(matrix[i].bit);
        // enable pull-up resistor
        *(matrix[i].port) |=  _BV(matrix[i].bit);
    }
}

i8 matrix_keyboard_getkey(void)
{
    for (i8 row = 0; row < 4; row++) {
        // set row to low state
        *(matrix[row].port) &= ~_BV(matrix[row].bit);
        for (i8 col = 4; col < 8; col++) {
            // check the state of the corresponding column pin
            u8 state = *(matrix[col].pin) & _BV(matrix[col].bit);
            if (state) {
                continue;
            }

            // restore row to high state
            *(matrix[row].port) |= _BV(matrix[row].bit);
            return matrix_map[row][col-4];
        }
        // set row to high state
        *(matrix[row].port) |= _BV(matrix[row].bit);
    }

    return -1;
}
