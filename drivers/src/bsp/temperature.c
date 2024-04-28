#include "temperature.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static i16 temperature_in_c;

static void adc_init()
{
    // mux line to use ADC15
    ADMUX |= _BV(MUX0) | _BV(MUX1) | _BV(MUX2);
    ADMUX &= ~(_BV(MUX3) | _BV(MUX4));
    ADCSRB |= _BV(MUX5);

    // select voltage reference to 1.1V
    ADMUX |= _BV(REFS1);
    ADMUX &= ~_BV(REFS0);

    // set ADC division factor to 128 - results in 16MHz/128=125kHz sampling frequency
    ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);

    // enable ADC interrupt
    ADCSRA |= _BV(ADIE);

    // turn on ADC
    ADCSRA |= _BV(ADEN);
}

#define TIMER_PRESCALER 1024

static void timer_init(i16 period)
{
    // Do not allow period greater than 4 seconds
    if (period > 4000) {
        period = 4000;
    }

    // Set CTC mode of operation
    TCCR1B |= (1 << WGM12);

    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1B &= ~(1 << CS11);

    // Set counter value
    OCR1A = (u16)((F_CPU / TIMER_PRESCALER * period) / 1000);

    // Enable timer interrupt
    TIMSK1 |= (1 << OCIE1A);
}

void temperature_init(i16 period)
{
    // set PG0 direction to output
    DDRG |= _BV(PG0);
    PORTG |= _BV(PG0);

    // set PK7 direction to input
    DDRK &= ~_BV(PK7);

    cli();
    adc_init();
    timer_init(period);
    sei();
}

i16 temperature_get()
{
    return temperature_in_c;
}

#define REFERENCE_TEMPERATURE 25
#define REFERENCE_VOLTAGE 750
#define VOLTAGE_PER_DEG_C 10

ISR(ADC_vect)
{
    // Get the converted value and calculate the temperature
    u16 voltage = ADCL | ((ADCH & 0x3) << 8);
    temperature_in_c = REFERENCE_TEMPERATURE + ((i16)(voltage - (u16)REFERENCE_VOLTAGE)) / VOLTAGE_PER_DEG_C;
}

ISR(TIMER1_COMPA_vect)
{
    // Start ADC conversion
    ADCSRA |= _BV(ADSC);
}
