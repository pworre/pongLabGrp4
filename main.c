#include <stdint.h>
#include <stdio.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/io_board.h"
#include "drivers/spi.h"
#include "drivers/oled.h"
#include "drivers/fonts.h"
#include "drivers/menu.h"
#include <util/delay.h>

#define TEST_PIN 1

int main(){
    //DDRC = 0b11111111;
    //DDRA = 0b11111111;
    

    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);

    sram_init();
    //SRAM_test();
    


    io_board_init();
    io_board_calibration();

    SPI_MasterInit();
    oled_init();

    while(1){
        uint8_t buttons;
        // Buttons consists of 3 bytes: RIGHT, LEFT, NAV
        SPI_MasterTransmit((0x04), IO_BOARD);
        for (int byte_nr = 0; byte_nr < 3; byte_nr++){
            switch(byte_nr){
                case 0: // RIGHT
                    buttons = SPI_read(0, IO_BOARD);
                case 1: // LEFT
                    buttons = SPI_read(0, IO_BOARD);
                case 2: // NAV
                    buttons = SPI_read(0, IO_BOARD);
                    if ((buttons & (1 << 4)) == 1){
                        printf("NAV-knapp trykket");
                    }
            }
        _delay_us(30);
        SPI_MasterTransmit((0x05), IO_BOARD);
        SPI_MasterTransmit(3, IO_BOARD);
        SPI_MasterTransmit(1);
    }
    }
    
    
    /*char* text = "NEW GAME";
    oled_write_string(text, 8, 0, 32);

    char* text2 = "LINJE 2!";
    oled_write_string(text2, 8, 2, 5);

    char* botn = "heilt nede!";
    oled_write_string(botn, 4, 7, 60);

    char* text0 = "TEST";
    oled_write_string(text0, 8, 3, 30);
    oled_draw(DIK, 5, 80);*/
    //main_menu();

    //oled_fill_screen();
    
    //oled_pos(31,62);
    //transmit_oled_data(10);

    //oled_goto_line(2);
    //transmit_oled_data(1);
    //while(1){
        //printf("%d", get_adc_data(1)); 
        //ADC_print();
        //oled_fill_screen();
    //}
    return 0;
}
