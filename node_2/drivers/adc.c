#include "adc.h"

#include "sam.h"

    // // Enable peripheral clock for PWM for PID36
    // PMC->PMC_PCER1 |= (1 << 4);
    
    // // motor shield "Servo SIG" uses PB13
    // PMC->PMC_PCER0 |= PMC_PCER0_PID12;
    // PIOB->PIO_PDR |= PIO_PDR_P13;
    // PIOB->PIO_ABSR |= PIO_ABSR_P13;
static void delay(volatile uint32_t n) {
    while(n--) __asm__("nop");
}

void adc_init_freerun(void){
    //setting up the A6-pin for ADC - CH6
    //Reset ADC, ready for init
    ADC->ADC_CR = ADC_CR_SWRST;

    // Enable peripheral clock
    PMC->PMC_PCER0 |= (1u << ID_PIOA);
    // Enable peripheral clock for ADC for PID37
    PMC->PMC_PCER1 |= (1 << 5);

    delay(1000);

    //disable PIO for pin A6 
    PIOA->PIO_PDR |= PIO_PDR_P3;
    PIOA->PIO_ABSR |= PIO_ABSR_P3;
    PIOA->PIO_PDR |= PIO_PDR_P24;
    PIOA->PIO_ABSR |= PIO_ABSR_P24;
    
    //disable write protection, kode: "ADC" in ascii
    ADC->ADC_WPMR = (0x414443u << ADC_WPMR_WPKEY_Pos);
    //ADC->ADC_WPMR &= ~(1 << ADC_WPMR_WPEN);

    //emable hardware trigger, freerun mode and ADC clock = MCL/32
    ADC->ADC_MR = 0;
    ADC->ADC_MR |= ((ADC_MR_FREERUN_ON) | (ADC_MR_PRESCAL(32)));
    ADC->ADC_MR &= ~(ADC_MR_SLEEP | ADC_MR_LOWRES | ADC_MR_TRGEN);
    ADC->ADC_MR |= ADC_MR_STARTUP_SUT64;
    ADC->ADC_MR |= ADC_MR_SETTLING_AST9;
    
    //enable comparison event interrupt
    ADC->ADC_IER |= ADC_IER_COMPE;
    NVIC_EnableIRQ(ADC_IRQn);

    //set compare mode to give interrupt when converted data is lower than threshold
    ADC->ADC_EMR &= ~(1 << ADC_EMR_CMPMODE_Pos);
    //compare on channel 6 / A6
    ADC->ADC_EMR |= (6 << ADC_EMR_CMPSEL_Pos);
    //set CH6 for LCDR

    //setting low threshold
    ADC->ADC_CWR |= ADC_LOWER_TRESHHOLD; //THIS VALUE MUST DE TESTED AND CHANGED!!!!
    //ADC->ADC_CR = ADC_CR_START;

    //select channel 6 as a ADC periferal (A6) i hope
    ADC->ADC_CHER = ADC_CHER_CH6;
}

void ADC_Handler(void){
    if (ADC->ADC_ISR & ADC_ISR_COMPE){
        uint32_t adc_value = ADC->ADC_LCDR;
        //global value lives??
        if(adc_value < ADC_LOWER_TRESHHOLD ){
            printf("You missed the ball!");
        }
    }else{
        __asm__("nop");
    }
    NVIC_ClearPendingIRQ(ADC_IRQn);
}