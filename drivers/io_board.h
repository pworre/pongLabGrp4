#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

#ifndef IO_BOARD_H
#define IO_BOARD_H

static int8_t scaled_values[4];

void io_board_init();
void ADC_read(void);
//uint8_t get_adc_data(uint8_t port);
void ADC_print(void);
void io_board_calibration(void);
void get_io_board_values(void);
void get_io_board_directions(void);


typedef enum {
    nav_btn,
    SL4,
    SL5,
    SL6,
    SL7,
    SR4, 
    SR5,
    SR6
} BUTTONS;

typedef enum {
    IDLE, 
    UP, 
    DOWN,
    LEFT,
    RIGHT,
} JOYSTICK_DIR;

volatile JOYSTICK_DIR joystick_dir;

#endif