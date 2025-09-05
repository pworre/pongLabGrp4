#ifndef CLOCK_H
#define CLOCK_H
#include <stdint.h>

#define COUNTER1 ((ATMEGA_COUNTER1_REG*)0x4E)

typedef struct {
    volatile uint8_t TCCR1B;
    volatile uint8_t TCCR1A;
} ATMEGA_COUNTER1_REG;

#endif

void counter1_init();