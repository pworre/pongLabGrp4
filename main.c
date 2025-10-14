#define F_CPU 4915200UL
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
#include "drivers/CAN_CTRL.h"
#include <util/delay.h>

int main(){

    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);

    sram_init();
    //SRAM_test();
    
    io_board_init();
    io_board_calibration();

    SPI_MasterInit();
    oled_init();

    CAN_CTRL_init();
    while (1){
        _delay_ms(2000);
        test_CAN_CTRL();
    }

    return 0;
}
