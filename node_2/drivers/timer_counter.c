#include "timer_counter.h"

PID_CONTROLLER pid_ctrl;

uint32_t score = 0;

void timer_counter_init(uint32_t tc_channel, uint32_t period){
    
    switch (tc_channel)
    {
    case 0:
        PMC->PMC_PCER0 |= PMC_PCER0_PID27; //enable clock
        PIOB->PIO_PDR |= (PIO_PB27); //enable periferal on PB27 / TC0
        PIOB-> PIO_ABSR |= (PIO_PB27); //select periferal B
        
        break;
    case 1:
        PMC->PMC_PCER0 |= PMC_PCER0_PID28; //enable clock
        PIOA->PIO_PDR |= (PIO_PA2); //enable periferal on PB27 / TC0
        PIOA->PIO_ABSR &= ~(PIO_PA2); //select periferal A
        break;
    
    default:
        break;
    }

    // Disable write-protection
    TC0->TC_WPMR = (0x54494D << TC_WPMR_WPKEY_Pos);

    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_WAVE; //sets waveform mode
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4; //prescaled the TC clock by 128
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_WAVSEL_UP_RC; // sets wavemode to UP_RC
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_ACPA_SET; //TIOA is set on compare with RA
    TC0->TC_CHANNEL[tc_channel].TC_CMR |= TC_CMR_ACPC_CLEAR; //TIOA is cleared on compare with RC
    //set RA and RC, init interrupts and enable the TC
    TC0->TC_CHANNEL[tc_channel].TC_RA = period / 2; //compare value on RA
    TC0->TC_CHANNEL[tc_channel].TC_RC = period; // compare value on RC
    TC0->TC_CHANNEL[tc_channel].TC_CCR |= TC_CCR_CLKEN | TC_CCR_SWTRG; // starts the timer counter and restart the timer
    TC0->TC_CHANNEL[tc_channel].TC_IER |= TC_IER_CPCS; //enables the RC compare interupt

    if (tc_channel == 0){
        
        NVIC_EnableIRQ(TC0_IRQn);
    }else if (tc_channel == 1){
        NVIC_EnableIRQ(TC1_IRQn);
    }
    else {
        printf("not a used timer counter!");
    }

    // enable write-protection
    TC0->TC_WPMR |= 1;
}

//write the interrupts handlers

void TC0_Handler(void){//TC0 is used for counting the score while playing, updates a global value
    score++;
    uint32_t reg = TC0->TC_CHANNEL[0].TC_SR;
    NVIC_ClearPendingIRQ(TC0_IRQn);
}
void TC1_Handler(void){//TC1 is used for updating the PID control for the motor in consistant time steps. 
    CAN_MESSAGE msg;
    pid_use_controller(&pid_ctrl, &msg);
    uint32_t reg = TC0->TC_CHANNEL[1].TC_SR;
    NVIC_ClearPendingIRQ(TC1_IRQn);
}
