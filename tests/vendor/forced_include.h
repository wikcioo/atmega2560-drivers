#pragma once

#include <stdint.h>

#define __AVR_ATmega2560__				//Define MCU
#define __AVR_LIBC_DEPRECATED_ENABLE__	//Avoid pragma errors
#define _AVR_SFR_DEFS_H_ 1				//Disable original sfr_defs.h

// Define the highest register address in the MCU
#define _HIGHEST_REGISTER_ADDR 0x136	// ATMEGA2560

// These global variables needs to be accessable from both C and C++
// Therefore they must be declared in C scope
#ifdef __cplusplus
extern "C" {
#endif
extern uint8_t __avr_reg[_HIGHEST_REGISTER_ADDR];
#ifdef __cplusplus
}
#endif

// Replace AVR registers with Array
#define _SFR_MEM8(mem_addr) (*(uint8_t *)(&__avr_reg[mem_addr]))
#define _SFR_IO8(io_addr) (*(uint8_t *)(&__avr_reg[io_addr]))
#define _SFR_MEM16(mem_addr) (*(uint16_t *)(&__avr_reg[mem_addr]))

#define SREG  (*(uint8_t *)(&__SREG))

// Helper macros
#define _BV(bit) (1<<(bit))

// Interrupt
#define _AVR_INTERRUPT_H_
#define ISR(vector, ...) void ISR_##vector(void)
#define sei() SREG |= _BV(SREG_I)
#define cli() SREG &= ~_BV(SREG_I)
