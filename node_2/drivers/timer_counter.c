#include "timer_counter.h"

void timer_counter_init(uint32_t tc_channel, uint32_t period){
    
    switch (tc_channel)
    {
    case 0:
        PMC->PMC_PCER0 |= PMC_PCER0_PID25; //enable clock
        PIOB->PIO_PDR |= (PIO_PB25); //enable periferal on PB27 / TC0
        PIOB-> PIO_ABSR |= (PIO_PB25); //select periferal B
        
        break;
    case 1:
        PMC->PMC_PCER0 |= PMC_PCER0_PID28; //enable clock
        PIOA->PIO_PDR |= (PIO_PA2); //enable periferal on PB27 / TC0
        PIOA->PIO_ABSR &= ~(PIO_PA2); //select periferal A
        break;
    
    case 2:
        PMC->PMC_PCER0 |= PMC_PCER0_PID29; //enable clock
        PIOA->PIO_PDR |= (PIO_PA5); //enable periferal on PB27 / TC0
        PIOA->PIO_ABSR |= ~(PIO_PA5); //select periferal A
    break;
    
    default:
        break;
    }

    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_WAVE; //sets waveform mode
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4; //prescaled the TC clock by 128
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_WAVSEL_UP_RC; // sets wavemode to UP_RC
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_ACPA_SET; //RA is set on compare
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_ACPA_CLEAR; //RC is cleared on compare
    //set RA and RC, init interrupts and enable the TC
}

//write the interrupts handlers