#include <gtest/gtest.h>

extern "C"
{
#include <avr/io.h>
#include <bsp/led_array.h>
}

class LedArrayTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        led_array_init();
    }
};

TEST_F(LedArrayTestFixture, led_array_init)
{
    ASSERT_EQ(0xFF, DDRA);
    ASSERT_EQ(0xFF, PORTA);
}

TEST_F(LedArrayTestFixture, led_array_set)
{
    led_array_set(LED1, LED_HIGH);
    ASSERT_EQ(0xFE, PORTA);
}

TEST_F(LedArrayTestFixture, led_array_toggle)
{
    led_array_toggle(LED1);
    ASSERT_EQ(0xFE, PORTA);
}

TEST_F(LedArrayTestFixture, led_array_clear)
{
    led_array_set(LED1, LED_HIGH);
    led_array_set(LED3, LED_HIGH);
    led_array_set(LED5, LED_HIGH);

    ASSERT_EQ(0xEA, PORTA);

    led_array_clear();

    ASSERT_EQ(0xFF, PORTA);
}
