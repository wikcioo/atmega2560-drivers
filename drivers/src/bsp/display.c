#include "display.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CLK_RCK 0
#define CLK_SCK 1
#define DASH_CODE 0x01

// XABC DEFG -> D
// 0111 1110 -> 0
// 0011 0000 -> 1
// 0110 1101 -> 2
// 0111 1001 -> 3
// 0011 0011 -> 4
// 0101 1011 -> 5
// 0101 1111 -> 6
// 0111 0000 -> 7
// 0111 1111 -> 8
// 0111 1011 -> 9

// Digit to code mapping
static u8 display_code_map[] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B };

static void _pulse_clock(u8 clk_bit)
{
    PORTB |= (1 << clk_bit);
    PORTB &= ~(1 << clk_bit);
}

static void _activate_digit(u8 i)
{
    PORTF |= 0x0F;
    PORTF &= ~(1 << (3-i));
}

static void _display_reset()
{
    PORTB |= (1 << 2);

    for (u8 i = 0; i < 4; i++)
    {
        // Fill shift register with zeros
        for (u8 j = 0; j < 8; j++)
            _pulse_clock(CLK_SCK);

        // Move to shift storage register
        _pulse_clock(CLK_RCK);

        // Activate next digit
        _activate_digit(i);
    }
}

static void _display_code(u8 code)
{
    // DP
    PORTB |= (1 << 2); // SI
    _pulse_clock(CLK_SCK);

    for (u8 j = 0; j < 7; j++)
    {
        u8 bit = (code >> j) & 1;
        if (bit)
            PORTB &= ~(1 << 2); // SI
        else
            PORTB |= (1 << 2); // SI

        _pulse_clock(CLK_SCK);
    }

    // Move to shift storage register
    _pulse_clock(CLK_RCK);
}

static u8 i = 0;
static b8 is_negative;
static i16 number;
static i16 original_number;
static b8 printed_dash = false;

static void _update_display()
{
    // Display dash if the number is negative and all digits have been printed already
    if (is_negative && number <= 0)
    {
        _display_code(DASH_CODE);
        _activate_digit(i);
        printed_dash = true;
    }
    else
    {
        // Extract the digit and convert it to display code
        u8 digit = number % 10;
        u8 code = display_code_map[digit];

        _display_code(code);
        _activate_digit(i);

        number /= 10;
        i++;
    }

    // If all digits have been extracted from the number, reset it to the original number
    if (number <= 0 && (!is_negative || printed_dash))
    {
        i = 0;
        number = original_number;
        printed_dash = false;
    }
}

#define INTERRUPT_FREQUENCY (60UL * 4UL)
#define PRESCALER_VALUE 1024UL
#define OCRA_VALUE F_CPU / PRESCALER_VALUE / INTERRUPT_FREQUENCY

ISR(TIMER2_COMPA_vect) {
    // Update one digit of the number
    _update_display();
}

void display_init()
{
    // Set output mode for PB0, PB1, PB2
    DDRB |= 0x07;
    // Set output mode for PF0, PF1, PF2, PF3
    DDRF |= 0x0F;
    // Set low state on PB0, PB1, PB2
    PORTB &= ~(0x07);
    // Set high state on PF0, PF1, PF2, PF3
    PORTF |= 0x0F;

    // Set CTC mode of operation
    TCCR2B |= (1 << WGM22);

    // Set prescaler to 1024
    TCCR2B |= (1 << CS12) | (1 << CS10);
    TCCR2B &= ~(1 << CS11);

    // Set counter value
    OCR2A = OCRA_VALUE;

    // Enable timer interrupt
    TIMSK2 |= (1 << OCIE2A);

    // Enable global interrupts
    sei();
}

void display_printint(i16 value)
{
    if ((value < 0 && -value > 999) || (value >= 0 && value > 9999)) {
        value = 0;
    }

    cli();
    i = 0;
    is_negative = value < 0;
    number = is_negative ? -value : value;
    original_number = number;
    _display_reset();
    sei();
}
