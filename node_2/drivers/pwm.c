#include "pwm.h"

#define SIG_pin 13

// PWM freq 50 Hz or 20 ms
// Dutycycle 0.9 -> 2.1 ms

// Period
#define CPRD 13125
#define dutycycle_percentage_lower_bound 5
#define dutycycle_percentage_upper_bound 10


void pwm_init(){
    // Enable peripheral clock for PWM for PID36
    PMC->PMC_PCER1 |= (1 << 4);
    
    // motor shield "Servo SIG" uses PB13
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    PIOB->PIO_PDR = PIO_PDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;
    PIOB->PIO_MDDR |= (1 << SIG_pin);   // output write


    // Disable write-protection
    PWM->PWM_WPCR = (0x50574D << 8) |(0b111111 << 2);
    PWM->PWM_WPCR &= ~((1 << 0) || (1 << 1));
    
    if (((PWM->PWM_WPSR & PWM_WPSR_WPHWS0) | (PWM->PWM_WPSR & PWM_WPSR_WPHWS1)) != 0){
        printf("WP-fault\r\n");
        return;
    }

    // ---------------- C L O C K ----------------
    // PWM_CLK = MCU_CLK
    PWM->PWM_CLK = 0;
    PWM->PWM_CLK |= (1 << 0) | (1 << 16);

    // --------- C H A N N E L - M O D E ---------
    //divice MCU CLK by 128 and sets polarity to start as high
    PWM->PWM_CH_NUM[1].PWM_CMR |= (0b111 << 0) | (1 << 9);

    PWM->PWM_CH_NUM[1].PWM_CMR &= ~(1 << 10);

    // Channel period
    PWM->PWM_CH_NUM[1].PWM_CPRD = CPRD;

    // Channdel dutycycle 50% for now
    PWM->PWM_CH_NUM[1].PWM_CDTY = ((CPRD) / 2);

    // PWM Enable
    PWM->PWM_ENA = PWM_ENA_CHID1;

    // Write protection enabled except WPVS
    PWM->PWM_WPCR |= 1;

    return;
}

void pwm_set_dutycycle(uint32_t dutycycle_percentage){
    if (dutycycle_percentage > dutycycle_percentage_upper_bound){
        dutycycle_percentage = dutycycle_percentage_upper_bound;
    }
    else if ( dutycycle_percentage < dutycycle_percentage_lower_bound){
        dutycycle_percentage = dutycycle_percentage_lower_bound;
    }
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = (dutycycle_percentage * CPRD) / 100;
}