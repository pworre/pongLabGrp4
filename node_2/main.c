#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/tests.h"
#include "drivers/pwm.h"
#include "drivers/adc.h"
#include "drivers/timer_counter.h"
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

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    pwm_init();
    uart_init(84000000, 9600);

    uint32_t can_br = 250000; //not used
    uint8_t num_tx_mb = 2;
    uint8_t num_rx_mb = 2;
    //can_init_def_tx_rx_mb(can_br);
    can_init(can_br, num_tx_mb, num_rx_mb);
    //adc_init_freerun();
    

    // uint32_t primask = __get_PRIMASK();
    // if (primask == 0){
    //     printf("Global interrupt enabled\r\n");
    // }
    timer_counter_init(0, 656250); //sett score TC, 1 poeng per sek ca.
    __enable_irq();
    
    while (1)
    {
        controll_servo_with_joystick_test();
        //send_can_msg_test(1);
        //recive_can_msg_test(0);


        
    }
}
