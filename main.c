#include <stdint.h>
#include <stdio.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/io_board.h"
#include "drivers/spi.h"
#include "drivers/oled.h"

#define TEST_PIN 1

int main(){
    //DDRC = 0b11111111;
    //DDRA = 0b11111111;
    

    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);

    //sram_init();
    //SRAM_test();


    io_board_init();
    io_board_calibration();

    SPI_MasterInit();
    oled_init();
    oled_pos(31,62);
    transmit_oled_data(10);
    oled_fill_screen();
    

    //oled_goto_line(2);
    //transmit_oled_data(1);
    while(1){
        //printf("%d", get_adc_data(1)); 
        //ADC_print();
        //oled_fill_screen();
    }
    return 0;

}
