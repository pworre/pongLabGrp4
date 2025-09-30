#include "oled.h"


uint8_t oled_current_page[128];

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
        transmit_oled_command(0xB0 | page); // Sett page-adresse
        transmit_oled_command(0x00 | 2);        // Kolonne laveste nibble
        transmit_oled_command(0x10 | 10);        // Kolonne høyeste nibble

        for (uint8_t col = 0; col < 128; col++) {
            transmit_oled_data(0b11001100); // Sett alle piksler i kolonnen
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
    transmit_oled_command(0xff);
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
void oled_goto_page(uint8_t page){
    // makes valid max_line
    if (page > 8){
        return;
    }
    transmit_oled_command(0xb0 + page);
}

void oled_goto_column(uint8_t column){
    if (column > 127 || column < 0) {
        return;
    }
    transmit_oled_command(0x00 + (column & 0b00001111));
    transmit_oled_command(0x10 + (column >> 4));
}

void oled_clear_line(uint8_t line){
    for (uint8_t page = 0; page < 8; page++) {
        transmit_oled_command(0xB0 | page); // Sett page-adresse
        transmit_oled_command(0x00);        // Kolonne laveste nibble
        transmit_oled_command(0x10);        // Kolonne høyeste nibble
        for (uint8_t col = 0; col < 128; col++) {
            transmit_oled_data(0x00); // Sett alle piksler i kolonnen
        }
}
}
void oled_pos(uint8_t page, uint8_t column) {
    oled_goto_page(page);
    oled_goto_column(column);
} 
void oled_write_string(char* ptr, uint8_t font_size, uint8_t page, uint8_t col){

    uint8_t char_count = 0;
    // Reset current page buffer
    for (int i = 0; i < 128; i++){
        oled_current_page[i] = 0;
    }


    while (*ptr != '\0' ){
        switch (font_size){
            case 4:
                for (uint8_t i=0; i < 4; i++){
                    oled_current_page[col + 4*char_count + i] =  pgm_read_byte(&font4[*ptr - 32][i]); //font4[*ptr - 32][i];
                }
                break;
            case 5:
                for (uint8_t i=0; i < 5; i++){
                    oled_current_page[col + 5*char_count + i] = pgm_read_byte(&font5[*ptr - 32][i]); //font5[*ptr - 32][i];
                }
                break;
            case 8:
                for (uint8_t i=0; i < 8; i++){
                    oled_current_page[col + 8*char_count + i] = pgm_read_byte(&font8[*ptr - 32][i]); //font8[*ptr - 32][i];
                }
                break;
            default:
            break;

        }
        char_count++;
        ptr++;
    }
    
    //oled_print_page(page);
    for (int i = 0; i < (char_count * font_size); i++){ // OBS!!!
        oled_write_byte(page, (col + i), oled_current_page[col + i]);
    }
}



void oled_print_page(uint8_t page){
    oled_goto_page(page);
    for (uint8_t col = 0; col < 128; col++) {
        transmit_oled_data(oled_current_page[col]); // Sett alle piksler i kolonnen
    }
}



void oled_write_byte(uint8_t page, uint8_t col, uint8_t byte){
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint8_t offset = page * 128 + col;
    ext_ram[offset] = byte;
}

void oled_get_page(uint8_t page){
    // Reset current page buffer
    for (int i = 0; i < 128; i++){
        oled_current_page[i] = 0;
    }

    volatile char *ext_ram = (char *) 0x1800;
    uint8_t offset = 128 * page;
    for (uint8_t i = 0; i<128; i++){
        oled_current_page[i] = ext_ram[offset + i];
    }
}

void oled_update_screen(void){
    for (uint8_t page = 0; page < 8; page++){
        oled_get_page(page);
        oled_print_page(page);
    }
}