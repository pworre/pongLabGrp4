#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/tests.h"
#include "drivers/pwm.h"
#include "drivers/adc.h"
#include "drivers/timer_counter.h"
#include "drivers/encoder.h"
#include "drivers/motor.h"
#include "drivers/solenoide.h"
#include "drivers/time.h"
#include "drivers/pid_controller.h"
#include "drivers/the_game.h"
//#include <util/delay.h>

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

    // uint32_t primask = __get_PRIMASK();
    // if (primask == 0){
    //     printf("Global interrupt enabled\r\n");
    // }

    
int main(){
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    
    pwm_init();
    uart_init(84000000, 9600);

    printf("MAIN\r\n");

    uint32_t can_br = 250000;
    uint8_t num_tx_mb = 2;
    uint8_t num_rx_mb = 2;
    can_init(can_br, num_tx_mb, num_rx_mb);

    adc_init_freerun();
    solenoide_init();
    encoder_init();
    motor_init();
    
    encoder_calibrate();
    timer_counter_init(0, 656250); //sett score TC, 1 poeng per sek ca.
    timer_counter_init(1, 656250 / 100); 


    int32_t duty_cycle = 0;
    uint32_t is_R5_pressed = 1;
    uint32_t i = 0;

    // PID
    float T = 0.01;
    float K_p = 0.15;
    float K_i = 0.02;
    float K_d = 0;
    pid_init(&pid_ctrl, K_p,  K_i, K_d, T);

    __enable_irq();
    printf("Ready to start PONG!\r\n");
    play_the_game(&pid_ctrl);
}

    // for(volatile uint32_t i = 0; i < 3000; i++){
    //     __asm__("nop");
    // }
