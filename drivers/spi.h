#include <stdio.h>
#include <avr/io.h>

#ifndef SPI_H
#define SPI_H

#define D_C 4
#define SS_OLED 3
#define SS_IO_BOARD 2
#define SS_ARDUINO 1 //ikke koblet ennå

typedef enum {
    IO_BOARD,
    OLED,
    ARDUINO
} SLAVES;

void SPI_MasterInit(void);
void SPI_MasterTransmit(uint8_t data, SLAVES slave);
uint8_t SPI_read(uint8_t command, SLAVES slave);

#endif