#include "pwm.h"

#define SIG_pin 13

// PWM freq 50 Hz or 20 ms
// Dutycycle 0.9 -> 2.1 ms

// Period
#define pwm_period 1680000

void pwm_init(){
    // Enable peripheral clock for PWM
    PMC->PMC_PCER1 |= (1 << 36);
    
    // motor shield "Servo SIG" uses PB13
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    PIOB->PIO_ABSR |= (1 << SIG_pin);
    PIOB->PIO_PDR |= (1 << SIG_pin);
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
    PWM->PWM_CH_NUM[0].PWM_CMR = 0;

    // Channel period
    PWM->PWM_CH_NUM[0].PWM_CPRD = pwm_period;

    // Channdel dutycycle
    PWM->PWM_CH_NUM[0].PWM_CDTY = (pwm_period / 2);

    // PWM Enable
    PWM->PWM_ENA = PWM_ENA_CHID0;

    // Write protection enabled except WPVS
    PWM->PWM_WPCR |= 1;

    return;
}