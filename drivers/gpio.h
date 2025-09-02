#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#define GPIO ((ATMEGA_GPIO_REG*)0x20)

typedef struct {
    volatile uint8_t RESERVED0[25];
    volatile uint8_t PINA;
    volatile uint8_t DDRA;
    volatile uint8_t PORTA;
} ATMEGA_GPIO_REG;

#endif