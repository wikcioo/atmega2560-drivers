#include <gtest/gtest.h>

extern "C"
{
#include <avr/io.h>
#include <switch.h>
}

class SwitchTestFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        switch_init();
    }
};

TEST_F(SwitchTestFixture, switch_init)
{
    ASSERT_EQ(0, DDRC & (0x1F));
    ASSERT_EQ(0, DDRD & (0x03));
}

TEST_F(SwitchTestFixture, switch_get_pinc)
{
    // Pull pin PD3 high, which means that it is NOT pressed
    PINC |= (1 << 0);
    // Poll the state of SWITCH1 (PC0)
    b8 state = switch_get(SWITCH1);
    // Assert the state is off
    ASSERT_EQ(state, SWITCH_OFF);
}

TEST_F(SwitchTestFixture, switch_get_pind)
{
    // Ground pin PD3, which means that it is pressed
    PIND &= ~(1 << 3);
    // Poll the state of SWITCH8 (PD3)
    b8 state = switch_get(SWITCH8);
    // Assert the state is on
    ASSERT_EQ(state, SWITCH_ON);
}
