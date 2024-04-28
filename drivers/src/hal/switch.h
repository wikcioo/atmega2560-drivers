#pragma once

#include "defines.h"

// #SWITCH_NUMBERS
#define SWITCH1 0
#define SWITCH2 1
#define SWITCH3 2
#define SWITCH4 3
#define SWITCH5 4
#define SWITCH6 5
#define SWITCH7 2
#define SWITCH8 3

// #SWITCH_STATES
#define SWITCH_OFF 0
#define SWITCH_ON  1

/*
 * Initialize switches
 */
void switch_init();

/*
 * @param switch_no Switch number. See #SWITCH_NUMBERS for possible values
 * @return Switch state. See #SWITCH_STATES for possible values
 */
b8 switch_get(b8 switch_no);
