#include "encoder.h"

// Pins PC25-26
// Timer-counter 6

// RANGE = 5612

void init_encoder(void){
    PMC->PMC_PCER1 |= PMC_PCER1_PID33; //enable clock for TC6
    PIOC->PIO_PDR |= (PIO_PC25 | PIO_PC26); //disable periferal
    PIOC-> PIO_ABSR |= (PIO_PC25 | PIO_PC26); //select periferal B

    // disable Write-protection
    TC2->TC_WPMR = (0x54494D << TC_WPMR_WPKEY_Pos);

    TC2->TC_CHANNEL[0].TC_CMR &= ~TC_CMR_WAVE; // enable capture mode
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0; // Sets XC0 as clock
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ETRGEDG_RISING; // Trigger edge selection
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ABETRG; // TIOA as external trigger

    // Quadratic Mode
    TC2->TC_BMR = 0;
    TC2->TC_BMR |= TC_BMR_QDEN;
    TC2->TC_BMR |= TC_BMR_POSEN;
    TC2->TC_BMR |= TC_BMR_EDGPHA;
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    // Starts TC2
    TC2->TC_CHANNEL[0].TC_CCR |= (TC_CCR_CLKEN | TC_CCR_SWTRG);

    // enable write-protection
    TC2->TC_WPMR = ((0x54494D << TC_WPMR_WPKEY_Pos) | (1 << 0));
}

uint32_t read_encoder(void){
    return (TC2->TC_CHANNEL[0].TC_CV);
}

void encoder_calibrate(void){
    //drives the motor to whatever side is the minumum value, and starts the encoder counter from there
    //now we only get positive values, and since we know many encoder steps there is, we now the pos. 
    motor_setdir(LEFT);
    motor_setpower(50); //TODO: set an apropriate motor power
    int32_t last_encoder_val = 10000;
    int32_t encoder_val = 0;
    while (last_encoder_val > encoder_val){
        for(volatile uint32_t i = 0; i < 10000; i++){
            __asm__("nop");
        }
        last_encoder_val = encoder_val;
        encoder_val = read_encoder();
    }
    //stop motor and reset the TC2 counter / encoder counter
    motor_setpower(0);
    TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
}