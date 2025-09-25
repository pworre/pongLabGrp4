#include <stdint.h>
#include <stdio.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/io_board.h"

#define TEST_PIN 1

int main(){
    //DDRC = 0b11111111;
    //DDRA = 0b11111111;
    

    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);

    sram_init();
    SRAM_test();


    io_board_init();
    io_board_calibration();
    while(1){
        //printf("%d", get_adc_data(1)); 
        ADC_print();
    }
    return 0;
}
