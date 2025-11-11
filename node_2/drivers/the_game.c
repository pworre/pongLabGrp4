#include "the_game.h"

JOYSTICK joystick;

void play_the_game(PID_CONTROLLER *pid_ctrl){
    uint32_t from_main = 1;
    GAME game; 
    game.state = MENU;

    CAN_MESSAGE can_message;
    can_message.id = 0xFF;

    CAN_MESSAGE can_msg_send;

    while (1){
        // RECEIVES CAN_MESSAGE
        can_receive(&can_message, 1);
        //printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", can_message.id, can_message.data_length, can_message.data[0], can_message.data[1]);
        can_sort_message(&game, &can_message);
        //printf("STATE: %d\r\n", game.state);

        switch (game.state)
        {
            case MENU:
                pid_ctrl->toggle = 0;
            break;
            
            case PLAY:
            
            
            if (!from_main){
                encoder_calibrate();
                pid_ctrl->toggle = 1;
                pid_ctrl->reference = ENCODER_MAX/2;
                for(volatile uint32_t i = 0; i < 4000000; i++){
                    __asm__("nop");
                }
            }
            pid_ctrl->toggle = 1;
           
            for(volatile uint32_t i = 0; i < 3000; i++){
                __asm__("nop");
            }
            
            printf("ENTERED PLAY-STATE\r\n");
            uint32_t is_R5_pressed = 0;
            uint32_t duty_cycle = 0;
            volatile uint32_t i = 0;
            goals = 0;
            score = 0;

            while(game.state == PLAY){

                // RECEIVES CAN_MESSAGE
                can_receive(&can_message, 1);
                can_sort_message(&game, &can_message);

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

                //printf("PID-output: %d\r\n", pid_ctrl->controller_output);
                if (i % 100000 == 0){
                    // SEND GAME_INFO TO NODE_1
                    printf("sends can to node 1\r\n");
                    can_msg_send.id = 2;
                    can_msg_send.data_length = 2;
                    can_msg_send.data[0] = goals;
                    can_msg_send.data[1] = score;
                    can_send(&can_msg_send, 0);
                }
                i++;
            }
            printf("GAME OVER!");
                for(volatile uint32_t i = 0; i < 30000; i++){
                    __asm__("nop");
                }
            // goals = 0;
            // score = 0;
            from_main = 0;
            pid_ctrl->toggle = 0;
            break;
        
        default:
            break;
        }
    }
}

void game_init(PID_CONTROLLER *pid_ctrl){
    return;
}