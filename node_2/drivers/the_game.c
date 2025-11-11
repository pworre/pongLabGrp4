#include "the_game.h"

void play_the_game(PID_CONTROLLER *pid_ctrl){
    GAME game; 
    game.state = MENU;

    JOYSTICK joystick;
    CAN_MESSAGE can_message;
    CAN_MESSAGE can_msg_send;
    GAME_STATES game_state = MENU;

    while (1){
        // RECEIVES CAN_MESSAGE
        can_receive(&can_message, 1);
        can_sort_message(&game, &joystick, &can_message);

        switch (game_state)
        {
            case MENU:
            break;
            
            case PLAY:
            printf("ENTERED PLAY-STATE\r\n");
            uint32_t is_R5_pressed = 0;
            uint32_t duty_cycle = 0;

            while(game.state == PLAY){
                // RECEIVES CAN_MESSAGE
                can_receive(&can_message, 1);
                can_sort_message(&game, &joystick, &can_message);

                // MOTOR
                duty_cycle = (((dutycycle_upper_bound - dutycycle_lower_bound ) * joystick.y_axis) / 200) + dutycycle_middle;
                pwm_set_dutycycle(duty_cycle);

                // SOLENOIDE
                if ((can_message.data[2] & (1 << 4)) && is_R5_pressed){// if SR% is pressed
                    solenoide_activate();
                    is_R5_pressed = 0;
                } else if (can_message.data[2] & (1 << 4)){
                    is_R5_pressed = 0;
                } else {
                    is_R5_pressed = 1;
                }

                // PID-CONTROL
                pid_use_controller(&pid_ctrl, &joystick);

                // SEND GAME_INFO TO NODE_1
                can_msg_send.id = 2;
                can_msg_send.data_length = 2;
                can_msg_send.data[0] = goals;
                can_msg_send.data[1] = score;
                can_send(&can_msg_send, 0);
            }
            break;
        
        default:
            break;
        }
    }
}

void game_init(PID_CONTROLLER *pid_ctrl){
    adc_init_freerun();
    solenoide_init();
    encoder_init();
    motor_init();
    encoder_calibrate();
    score_init();


    int32_t duty_cycle = 0;
    uint32_t is_R5_pressed = 1;
    uint32_t i = 0;

    // PID
    float T = 0.01;
    float K_p = 0.15;
    float K_i = 0.02;
    float K_d = 0;
    timer_counter_init(1, 656250 / 100); 
    pid_init(&pid_ctrl, K_p,  K_i, K_d, T);
    __enable_irq();
}

void score_init(void){
    timer_counter_init(0, 656250); //sett score TC, 1 poeng per sek ca.
}