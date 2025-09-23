#include <avr/io.h>
#include <util/delay.h>

#ifndef IO_BOARD_H
#define IO_BOARD_H



void io_board_init();
void ADC_read(void);
//uint8_t get_adc_data(uint8_t port);
void ADC_print(void);
void io_board_calibration(void);
void get_io_board_values(void);


#endif