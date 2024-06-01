#include <gtest/gtest.h>

extern "C"
{
#include <avr/io.h>
#include <bsp/temperature.h>
}

#define TEMPERATURE_SAMPLING_PERIOD_MS 1000

class TemperatureTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        temperature_init(TEMPERATURE_SAMPLING_PERIOD_MS);
    }
};

TEST_F(TemperatureTestFixture, temperature_init)
{
    ASSERT_EQ(1, DDRG & _BV(PG0));
    ASSERT_EQ(1, PORTG & _BV(PG0));
    ASSERT_EQ(0, DDRK & _BV(PK7));

    // test timer registers
    ASSERT_EQ(0b10000111, ADMUX);
    ASSERT_EQ(0b00001000, ADCSRB);
    ASSERT_EQ(0b10001111, ADCSRA);

    // test adc registers
    ASSERT_EQ(0b000001101, TCCR1B);
    // given 1 second sampling period, OCR1A should have value 0x3D09 (0b11110100001001)
    ASSERT_EQ(0b11110100001001, OCR1A);
    ASSERT_EQ(0b00000010, TIMSK1);
}

#define REFERENCE_TEMPERATURE 25
#define REFERENCE_VOLTAGE 750
#define VOLTAGE_PER_DEG_C 10

i16 adc_vect(void)
{
    u16 voltage = ADCL | ((ADCH & 0x3) << 8);
    i16 temperature_in_c = REFERENCE_TEMPERATURE + ((i16)(voltage - (u16)REFERENCE_VOLTAGE)) / VOLTAGE_PER_DEG_C;
    return temperature_in_c;
}

TEST_F(TemperatureTestFixture, temperature_test_conversion)
{
    // those values in ADC register should result in 25 deg C of temperature after conversion
    ADCL = 236;
    ADCH = 2;

    i16 expected_temperature = 25;

    i16 temperature = adc_vect();
    ASSERT_EQ(temperature, expected_temperature);
}
