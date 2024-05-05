#include <util/delay.h>

// Include driver header files
#include <bsp/switch.h>
#include <bsp/display.h>
#include <bsp/led_array.h>
#include <bsp/temperature.h>
#include <bsp/matrix_keyboard.h>

// Define some constants
#define TEMPERATURE_SAMPLING_PERIOD_MS 1000
#define VALVE_LED LED6
#define PUMP_LED LED7

// Create variables for holding global state information with default values
static i8 low_temperature = 23;
static i8 high_temperature = 26;
static u8 last_pressed_switch = SWITCH3;

// This function executes when the user presses a key on the matrix keyboard
// while viewing temperature low (last_pressed_switch = SWITCH1) or viewing temperature high (last_pressed_switch = SWITCH2)
static void handle_temperature_change()
{
    i8 key;
    u16 new_temperature_value = 0;
    do {
        _delay_ms(30);
        key = matrix_keyboard_getkey();
        display_printint(new_temperature_value);

        // If the key is not pressed or the key value is one of the letters A-D
        // then continue to the next iteration
        if (key == KEY_NOT_PRESSED || (key >= 'A' && key <= 'D')) {
            continue;
        }

        // Only update the new temperature value if a number is pressed
        if (key >= '0' && key <= '9') {
            // Convert ascii value of the key to numeric value
            i8 number = key - 48;
            new_temperature_value = new_temperature_value * 10 + number;

            // If the temperature value exceeds 9999, it resets to 0
            // The user can then enter a new value from the beginning.
            if (new_temperature_value > 9999) {
                new_temperature_value = 0;
            }
        }

        // Wait until the user releases the key, which prevents key repeattttt
        i8 control_key = key;
        while (control_key != KEY_NOT_PRESSED) {
            control_key = matrix_keyboard_getkey();
            _delay_ms(30);
        }
    } while (key != '#' && key != '*'); // loop until either '#' or '*' is pressed

    // If the key pressed is '*' it means that the user wants to abort the operation
    // We also do NOT allow values greater than 127 to be assigned to low or high temperature
    if (key == '*' || new_temperature_value >= 128)
        return;

    // Change current high or low temperature values based on switch value
    if (last_pressed_switch == SWITCH1) {
        low_temperature = (u8)new_temperature_value;
    } else if (last_pressed_switch == SWITCH2) {
        high_temperature = (u8)new_temperature_value;
    }
}

static void wait_for_key_released(void)
{
    i8 key;
    do {
        key = matrix_keyboard_getkey();
        _delay_ms(30);
    } while (key == '#' || key == '*');
}

int main()
{
    // Initialize drivers
    switch_init();
    display_init();
    led_array_init();
    matrix_keyboard_init();
    temperature_init(TEMPERATURE_SAMPLING_PERIOD_MS);

    // Create local variable holding current state
    i8 is_valve_open = true;
    i8 is_pump_running = false;
    i16 temperature = 0;

    for (;;) {
        // Check for switch presses
        if (switch_get(SWITCH1)) {
            last_pressed_switch = SWITCH1;
        } else if (switch_get(SWITCH2)) {
            last_pressed_switch = SWITCH2;
        } else if (switch_get(SWITCH3)) {
            last_pressed_switch = SWITCH3;
        }

        led_array_clear();

        // Check for temperature changes
        if (temperature < low_temperature) {
            is_valve_open = true;
            is_pump_running = false;
        } else if (temperature > high_temperature) {
            is_valve_open = false;
            is_pump_running = true;
        }

        if (is_valve_open) {
            led_array_set(VALVE_LED, LED_HIGH);
        }

        if (is_pump_running) {
            led_array_set(PUMP_LED, LED_HIGH);
        }

        // Check for matrix key presses
        i8 key = matrix_keyboard_getkey();
        if (last_pressed_switch == SWITCH1) {
            display_printint(low_temperature);
            led_array_set(LED1, LED_HIGH);
            if (key != KEY_NOT_PRESSED) {
                handle_temperature_change();
            }
            // Wait until either '#' or '*' key is released
            wait_for_key_released();
        } else if (last_pressed_switch == SWITCH2) {
            display_printint(high_temperature);
            led_array_set(LED2, LED_HIGH);
            if (key != KEY_NOT_PRESSED) {
                handle_temperature_change();
            }
            // Wait until either '#' or '*' key is released
            wait_for_key_released();
        } else if (last_pressed_switch == SWITCH3) {
            temperature = temperature_get();
            display_printint(temperature);
            led_array_set(LED3, LED_HIGH);
        } else {
            // The execution flow should never reach this statement
            // Print error code (-1) on the display if that happens
            display_init(-1);
        }

        _delay_ms(200);
    }

    return 0;
}
