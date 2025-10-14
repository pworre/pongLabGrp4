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

    //while(1){
      //  get_io_board_values();
        /*
        uint8_t buttons[3];
        // Buttons consists of 3 bytes: RIGHT, LEFT, NAV
        SPI_MasterTransmit((0x04), IO_BOARD);
        _delay_us(DELAY_COMMAND_DATA);
        buttons[0] = SPI_read(IO_BOARD);
        _delay_us(DELAY_DATA_DATA);    
        buttons[1] = SPI_read(IO_BOARD);
        _delay_us(DELAY_DATA_DATA);
        buttons[2] = SPI_read(IO_BOARD);
        _delay_us(DELAY_DATA_DATA);
        PORTB |= ((1 << SS_OLED) | (1 << SS_CAN) | (1 << SS_IO_BOARD));

        printf("%d              %d              %d\r\n", buttons[0], buttons[1], buttons[2]);
        _delay_ms(500);
        //printf("%d\r\n", buttons);
        _delay_us(DELAY_DATA_DATA);

        io_board_led_power(2, 1);
        io_board_led_pwm(2, 100);

        */
        

       /*
        for (uint8_t state = 0; state < 2; state++){
            for (uint8_t i = 0; i < 5; i++){
                io_board_led_power(i, state);
                _delay_us(1000);
                io_board_led_pwm(i, 255);
                _delay_us(1000);
            }
            printf("LED\r\n");
            _delay_ms(1000);
        }
       printf("I while\r\n");
       io_board_led_power(2, 0b11111111);
       */
   // }

    main_menu_state = NEW_GAME;
    main_menu();

    return 0;
}
