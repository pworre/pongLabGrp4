#include "motor.h"

#define PWM_PIN 12 // PB12
#define CPRD 6000 //13125 is 5kHz, 6000 gives 10.94kHz


void motor_init(void){
    // Sets direction pin (PC23) as output
    PMC->PMC_PCER0 |= PMC_PCER0_PID13;
    PIOC->PIO_PER |= (1 << PWM_PIN);
    PIOC->PIO_OER |= (PIO_OER_P23);
    PIOC->PIO_CODR |= (PIO_CODR_P23);

    // PWM
    // Enable peripheral clock for PWM for PID36
    PMC->PMC_PCER1 |= (1 << 4);
    
    // motor shield "PWM SIG" uses PB12
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    PIOB->PIO_PDR |= PIO_PDR_P12;
    PIOB->PIO_ABSR |= PIO_ABSR_P12;
    PIOB->PIO_MDDR |= (1 << PWM_PIN);   // output write


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
    PWM->PWM_CH_NUM[0].PWM_CMR |= (0b111 << 0) | (1 << 9);

    PWM->PWM_CH_NUM[0].PWM_CMR &= ~(1 << 10);

    // Channel period
    PWM->PWM_CH_NUM[0].PWM_CPRD = CPRD;

    // Channel dutycycle 50% for now
    PWM->PWM_CH_NUM[0].PWM_CDTY = 0;

    // PWM Enable
    PWM->PWM_ENA = PWM_ENA_CHID0;

    // Write protection enabled except WPVS
    PWM->PWM_WPCR |= 1;
}

void motor_setdir(DC_DIRECTIONS dir){
    switch(dir){
        case RIGHT:
            PIOC->PIO_SODR |= (PIO_SODR_P23);
            break;
        case LEFT:
            PIOC->PIO_CODR |= (PIO_CODR_P23);    
            break;
        default:
            break;
    }
}

// Set power in percentage
void motor_setpower(uint32_t power){
    if (power > 100){
        //printf("Power setting not within limits!\r\n");
        power = 100;
    }

    uint32_t motor_cdty = (CPRD * power) / 100;
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = motor_cdty;
    //printf("motor cdty: %d\r\n", motor_cdty);
}