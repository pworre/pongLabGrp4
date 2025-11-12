#define F_CPU 4915200UL
#include "spi.h"
#include "fonts.h"

#ifndef OLED_H
#define OLED_H
//the OLED works by writing bytes to a buffer in the external SRAM, then update the OLED screen with this buffer. 
//oled_write_string() writes a string in the buffer spot you pick
//oled_update_screen() updates the OLED with the buffer
//this way you can change a small part of the oled at once in the buffer and the same function that updates the sreen does so every time

void transmit_oled_data(uint8_t data);
void transmit_oled_command(uint8_t cmd);

void oled_init(void);
void oled_reset(void);
void oled_home(void);
void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);
void oled_clear();
void oled_pos(uint8_t row, uint8_t column);
void oled_write_string(char* ptr, uint8_t font_size, uint8_t page, uint8_t col);
void oled_fill_screen(void);
void oled_print_page(uint8_t page);
void oled_write_byte(uint8_t page, uint8_t col, uint8_t byte);
void oled_get_page(uint8_t page);
void oled_update_screen(void);
void oled_draw(FIGURES fig, uint8_t page, uint8_t col);
void oled_write_inverted_string(char* ptr, uint8_t font_size, uint8_t page, uint8_t col);
void oled_clear_buffer(void);


#endif