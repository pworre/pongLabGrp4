#define F_CPU 4915200UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef SPI_H
#define SPI_H

#define D_C 4
#define SS_OLED 3
#define SS_IO_BOARD 2
#define SS_CAN 1 
#define DELAY_COMMAND_DATA 60 // us
#define DELAY_DATA_DATA 4 // us

typedef enum {
    IO_BOARD,
    OLED,
    CAN
} SLAVES;

//SPI_MasterTransmit() transmits a byte to the dedicated slave, 
//the slave select signal is NOT deselected after and SPI_slave_deselect() should be called when you are done

void SPI_MasterInit(void);
void SPI_MasterTransmit(uint8_t data, SLAVES slave);
uint8_t SPI_read(SLAVES slave);
void SPI_slave_deselect(void);

#endif