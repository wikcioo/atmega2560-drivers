#pragma once

#include "defines.h"

/*
 * Initialize the timer and set sampling period to 'period' in milliseconds
 * @param period Sampling period of the ADC
 */
void temperature_init(i16 period);

/*
 * Get current temperature value
 */
i16 temperature_get();
