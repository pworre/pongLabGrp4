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
    io_board_led_power(0, 0);
    io_board_led_power(1, 0);
    io_board_led_power(2, 0);
    io_board_led_power(3, 0);
    io_board_led_power(4, 0);
    io_board_led_power(5, 0);

    NODE_1_STATES node_1_state = MENU;
    main_menu_state = NEW_GAME;
    //settings_menu_state = CALIBRATION;
    uint8_t lives = 6;
    uint8_t score = 0;
    uint8_t goals = 0;

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

    msg_global.data[0] = 0;
    msg_global.data[1] = 0;
    msg_global.data[2] = 0;
    msg_global.data[3] = 0;
    msg_global.data[4] = 0;
    msg_global.data[5] = 0;
    msg_global.data[6] = 0;
    msg_global.data[7] = 0;
    msg_global.size = 8;
    msg_global.id = 0xff;
    
    while (1){
        switch (node_1_state)
        {
        case MENU:
            
            main_menu();
            node_1_state = PLAY;
            //send msg to node 2 that the game starts
            out_msg.id = 0;
            out_msg.size = 1;
            out_msg.data[0] = 1;
            can_send_msg(out_msg);
            draw_gameplay();
            break;
        
        case PLAY:
            can_recive_msg(&msg_global, 1);
            _delay_ms(10);
            get_io_board_values();
            out_msg.id = 1;
            out_msg.size = 5;
            out_msg.data[0] = scaled_values[0];
            out_msg.data[1] = scaled_values[1];
            out_msg.data[2] = buttons.right;
            out_msg.data[3] = buttons.left;
            out_msg.data[4] = buttons.nav;
            can_send_msg(out_msg);

            goals = msg_global.data[0]; //get number of goals scored
            lives = 6 - goals; 
            score = msg_global.data[1];
            update_gameplay(score, goals);

            //turn on light representing lives
            for (uint8_t light; light < 6; light++){
                if (light < lives){
                    io_board_led_power(light, 1);
                } else{
                    io_board_led_power(light, 0);
                }
            }

            if (goals == 6){ //change state when reviced game over signal. 
                node_1_state = MENU;
                update_highscore(score);
                lives = 6;
                goals = 0;
                score = 0;
                //send msg to node 2 that the game is over
                out_msg.id = 0;
                out_msg.size = 1;
                out_msg.data[0] = 0;
                can_send_msg(out_msg);
                //turn off lights
                io_board_led_power(0, 0);
                io_board_led_power(1, 0);
                io_board_led_power(2, 0);
                io_board_led_power(3, 0);
                io_board_led_power(4, 0);
                io_board_led_power(5, 0);
                //print game over
                print_gameover(score);
                _delay_ms(3000);
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