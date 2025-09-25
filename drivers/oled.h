#include "spi.h"

#ifndef OLED_H
#define OLED_H

void transmit_oled_data(uint8_t data);
void transmit_oled_command(uint8_t cmd);

void oled_init(void);
void oled_reset(void);
void oled_home(void);
void oled_goto_page(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_line(uint8_t line);
void oled_pos(uint8_t row, uint8_t column);
void oled_print(char* ptr);
void oled_fill_screen(void);

#endif