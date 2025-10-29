#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
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

    //Uncomment after including uart above
    //uart_init(/*cpufreq*/, /*baud*/);
    //printf("Hello World\n\r");

    /* code */
    uart_init(84000000, 9600);


    CanInit canInitParam = {
        .brp    = 41,
        .smp    = 0,
        .phase1 = 6,
        .phase2 = 5,
        .sjw    = 0,
        .propag = 1
    };
    // PRINT CAN_INIT to set REGISTER
    printf("CanInit: ");
    for (uint32_t bit = 32; bit >= 1; bit--){
        printf("%d", (canInitParam.reg >> (bit - 1)));
    } 
    printf("\r\n");

    //can_init_def_tx_rx_mb(can_br);
    can_init_def_tx_rx_mb(canInitParam)
    __enable_irq();

    uint32_t primask = __get_PRIMASK();
    if (primask == 0){
        printf("Global interrupt enabled\r\n");
    }


    // CAN receive info
    CAN_MESSAGE msg;
    uint32_t status_reg = 0;

    msg.data[0] = 8;
    msg.data_length=1;
    msg.id=47;

    while (1)
    {
        // NESTE GANG ! ! ! ! !
        // TEST NODE 2 LOOPBACKMODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // CAN0->CAN_MR |= CAN_MR_LPM;  // Loopback Mode


        uint32_t msr = CAN0->CAN_MB[0].CAN_MSR;
        uint32_t mmr = CAN0->CAN_MB[0].CAN_MMR;
        uint32_t mot = (mmr & CAN_MMR_MOT_Msk) >> CAN_MMR_MOT_Pos;
        uint32_t sr  = CAN0->CAN_SR;

        printf("TX_MB[%d]: MSR=0x%08lX MMR=0x%08lX MOT=%lu SR=0x%08lX\n\r", 
                0, msr, mmr, mot, sr);

        CAN0->CAN_MB[1].CAN_MCR = CAN_MCR_MTCR;

        can_send(&msg, 1);
        
        if (can_send(&msg, 1) == 1){
            printf("ikke sender\r\n");
        } else {
            printf("SENDER :)");
        }

        for(volatile uint32_t i = 0; i < 7000000; i++){
            __asm__("nop");
        }

        printf("ID = %d         Data = %d\r\n", msg.id, msg.data[0]);

        status_reg = CAN0->CAN_SR;
        for (uint32_t p = 32; p >= 1; p--){
            printf("%d", (status_reg >> (p-1)) & 1);
        }
        printf("\r\n");

    }
}