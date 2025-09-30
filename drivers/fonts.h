/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */
#ifndef FONTS_H_
#define FONTS_H_


#include <avr/pgmspace.h>

extern const uint8_t font8[95][8];
extern const uint8_t font5[95][5];
extern const uint8_t font4[95][4];


//void fonts_init(void);

#endif /* FONTS_H_ */


/* PREV. FONTS.C

#include "fonts.h"

void fonts_init(void){
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    for (uint16_t i = 0; i < 95; i++) {
        for (uint8_t j = 0; j < 8; j++)
        ext_ram[i*8 + j] = font8[i][j];
    }
    uint8_t offset = 95*8;
    for (uint16_t i = 0; i < 95; i++) {
        for (uint8_t j = 0; j < 5; j++)
        ext_ram[i*5 + j + offset] = font5[i][j];
    }
    offset += 95*5;
    for (uint16_t i = 0; i < 95; i++) {
        for (uint8_t j = 0; j < 4; j++)
        ext_ram[i*4 + j + offset] = font4[i][j];
    }
}

*/