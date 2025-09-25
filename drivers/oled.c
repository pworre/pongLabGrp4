#include "oled.h"

void transmit_oled_data(uint8_t data){
    //sett D/C til høy
    PORTB |= (1<<D_C);
    SPI_MasterTransmit(data, OLED);
}

void transmit_oled_command(uint8_t cmd){
    // D/C til lav
    PORTB &= ~(1<<D_C);
    SPI_MasterTransmit(cmd, OLED);
    //SPI_MasterTransmit((uint8_t)(cmd>>8), OLED);
    //SPI_MasterTransmit((uint8_t)(cmd & 0xff), OLED);
}

void oled_fill_screen(void) {
    for (uint8_t page = 0; page < 8; page++) {
        //transmit_oled_command(0xB0 | page); // Sett page-adresse
        //transmit_oled_command(0x00);        // Kolonne laveste nibble
        //transmit_oled_command(0x10);        // Kolonne høyeste nibble

        for (uint8_t col = 0; col < 128; col++) {
            transmit_oled_data(0xFF); // Sett alle piksler i kolonnen
        }
    }
}

void oled_init(void){
    transmit_oled_command(0xae); //display off
    transmit_oled_command(0xa1); //segment remap
    transmit_oled_command(0xda); //common pads hardware: alternative
    transmit_oled_command(0x12);
    transmit_oled_command(0xc8); //common output scan dicetion:como63-com~0
    transmit_oled_command(0xa8); //multiplex ration mode:63
    transmit_oled_command(0x3f);
    transmit_oled_command(0xd5); //display divide ratio/osc. freq. mode
    transmit_oled_command(0x80);
    transmit_oled_command(0x81); //contrast control
    transmit_oled_command(0x50);
    transmit_oled_command(0xd9); //set pre-charge period
    transmit_oled_command(0x21);
    transmit_oled_command(0x20); // | 0b10); //set memory addressing mode = Page adressing mode
    transmit_oled_command(0x02);
    transmit_oled_command(0xdb); //VCOM deselect level mode
    transmit_oled_command(0x30);
    transmit_oled_command(0xad); //master config
    transmit_oled_command(0x00);
    transmit_oled_command(0xa4); //ouy follows RAM contect
    transmit_oled_command(0xa6); //set normal display
    transmit_oled_command(0xaf); //display on
    for (uint8_t page = 0; page < 8; page++) {
        transmit_oled_command(0xB0 | page); // Sett page-adresse
        transmit_oled_command(0x00);        // Kolonne laveste nibble
        transmit_oled_command(0x10);        // Kolonne høyeste nibble

        for (uint8_t col = 0; col < 128; col++) {
            transmit_oled_data(0x00); // Sett alle piksler i kolonnen
        }
    }
}


void oled_reset(void);
void oled_home(void); // gå til (0,0)
void oled_goto_page(uint8_t line){
    // makes valid max_line
    if (line > 63){
        return;
    }
    transmit_oled_command(0xb0 + (line/8));
    transmit_oled_command(0x00);
    transmit_oled_command(0x10);
    /*uint8_t page = line/ 8;
    transmit_oled_command(0xB0 | page);
    transmit_oled_command(0x00 | line%8);
    transmit_oled_command(0x10 | line%8);
*/
}

void oled_goto_column(uint8_t column){
    if (column > 127 || column < 0) {
        return;
    }
    transmit_oled_command(0x00 + (column & 0x0f));
    transmit_oled_command(0x10 + (column >> 4));
}

void oled_clear_line(uint8_t line){
    int a;
}
void oled_pos(uint8_t row, uint8_t column) {
    oled_goto_page(row);
    oled_goto_column(column);
} 
void oled_print(char* ptr);