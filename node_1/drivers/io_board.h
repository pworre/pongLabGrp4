#define F_CPU 4915200UL
#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"
#include <stdlib.h>

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
void io_board_led_power(uint8_t led_nr, uint8_t state);
void io_board_led_pwm(uint8_t led_nr, uint8_t width);

typedef enum {
    IDLE, 
    UP, 
    DOWN,
    LEFT,
    RIGHT,
} JOYSTICK_DIR;

volatile JOYSTICK_DIR joystick_dir;

typedef struct __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
} Buttons;

Buttons buttons;

#endif