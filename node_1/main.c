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
#include "drivers/tests.h"

typedef enum {
    MENU, 
    PLAY,
} NODE_1_STATES;

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

    io_board_led_pwm(0, 70);
    io_board_led_pwm(1, 70);
    io_board_led_pwm(2, 70);
    io_board_led_pwm(3, 70);
    io_board_led_pwm(4, 70);
    io_board_led_pwm(5, 70);

    NODE_1_STATES node_1_state = MENU;
    main_menu_state = NEW_GAME;
    settings_menu_state = CALIBRATION;
    uint8_t lives = 6;

    //this can msg will either be io_board or game logic signals
    //io_board msg ID: 1
    //game logic ID: 0
    CAN_MESSAGE out_msg; 
    out_msg.data[0] = 0;
    out_msg.data[1] = 0;
    out_msg.data[2] = 0;
    out_msg.data[3] = 0;
    out_msg.data[4] = 0;
    out_msg.data[5] = 0;
    out_msg.data[6] = 0;
    out_msg.data[7] = 0;
    out_msg.size = 8;
    out_msg.id = 0xff;
    
    while (1){
        switch (node_1_state)
        {
        case MENU:
            main_menu();
            out_msg.id = 0;
            out_msg.size = 1;
            out_msg.data[0] = 1;
            node_1_state = PLAY;
            break;
        
        case PLAY:

            get_io_board_values();
            out_msg.id = 1;
            out_msg.size = 5;
            out_msg.data[0] = scaled_values[0];
            out_msg.data[1] = scaled_values[1];
            out_msg.data[2] = buttons.right;
            out_msg.data[3] = buttons.left;
            out_msg.data[4] = buttons.nav;
            can_send_msg(message);

            lives = 6 - msg_global.data[?]; //get number of goals scored


            //turn on light representing lives
            for (uint8_t light; light < 6; light++){
                if (light < lives){
                    io_board_led_power(light, 1);
                } else{
                    io_board_led_power(light, 0);
                }
            }

            if (msg_global.data[?] == ?){ //change state when reviced game over signal. 
                node_1_state = MENU;
                //update the highscore
            }
            break;

        default:
            break;
        }        
    }
    return 0;
}

        //send_can_msg_test(1);
        //recive_can_msg_test(1);
        // send_io_board_with_can();