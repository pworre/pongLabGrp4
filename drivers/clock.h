#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>

#define COUNTER1 ((ATMEGA_COUNTER1_REG*)0x48)

typedef struct {
    volatile uint8_t OCR1BL;
    volatile uint8_t OCR1BH;
    volatile uint8_t OCR1AL;
    volatile uint8_t OCR1AH;
    volatile uint8_t TCNT1L;
    volatile uint8_t TCNT1H;
    volatile uint8_t TCCR1B;
    volatile uint8_t TCCR1A;
    volatile uint8_t SFIOR;
    volatile uint8_t RESERVED[8];
    volatile uint8_t TIMSK;
} ATMEGA_COUNTER1_REG;

#endif

void counter1_init();