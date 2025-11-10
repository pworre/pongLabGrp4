#include "solenoide.h"

void solenoide_init(void){
    PMC->PMC_PCER0 |= PMC_PCER0_PID11;
    PIOA->PIO_PER |= PIO_PER_P3;
    PIOA->PIO_OER |= PIO_OER_P3;
    PIOA->PIO_MDDR |= PIO_MDDR_P3;

    PIOA->PIO_SODR |= PIO_SODR_P3;
}

void solenoide_activate(void){

    PIOA->PIO_CODR |= PIO_CODR_P3;

    for(volatile uint32_t i = 0; i < 200000; i++){
            __asm__("nop");
    }

    PIOA->PIO_SODR |= PIO_SODR_P3;
}