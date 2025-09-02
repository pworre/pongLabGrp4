#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>

#define CLOCK ((ATMEGA_CLOCK_REG*)0x80)

typedef struct {
    volatile uint8_t ICR3L;
    volatile uint8_t ICR3H;
    volatile uint8_t RESERVED[2];
    volatile uint8_t OCR3BL;
    volatile uint8_t OCR3BH;
    volatile uint8_t OCR3AL;
    volatile uint8_t OCR3AH;
    volatile uint8_t TCNT3L;
    volatile uint8_t TCNT3H;
    volatile uint8_t TCCR3B;
    volatile uint8_t TCCR3A;
} ATMEGA_CLOCK_REG;

#endif