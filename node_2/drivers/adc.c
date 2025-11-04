#include "adc.h"

#include "sam.h"

void adc_init_freerun(void){//setting up the A6pin for ADC
    //Reset ADC, ready for init
    ADC->ADC_CR |= ADC_CR_SWRST;

    // Enable peripheral clock for ADC for PID37
    PMC->PMC_PCER1 |= (1 << 5);

    //disable PIO for pin A6 
    PMC->PMC_PCER0 |= PMC_PCDR0_PID11;
    PIOA->PIO_PDR |= PIO_PA24X1_AD6;
    PIOA->PIO_ABSR |= PIO_PA24X1_AD6;
    
    //disable write protection, kode: "ADC" in ascii
    ADC->ADC_WPMR = (0x414443 << ADC_WPMR_WPKEY_Pos);
    ADC->ADC_WPMR &= ~(1 << ADC_WPMR_WPEN);

    //select channel 6 as a ADC periferal (A6) i hope
    ADC->ADC_CHER |= ADC_CHER_CH6;

    //emable hardware trigger, freerun mode and ADC clock = MCL/32
    ADC->ADC_MR |= (ADC_MR_TRGEN_EN) | (ADC_MR_FREERUN_ON) | (ADC_MR_PRESCAL(32));
    ADC->ADC_MR &= ~(ADC_MR_SLEEP | ADC_MR_LOWRES);
    
    //enable comparison event interrupt
    ADC->ADC_IER |= ADC_IER_COMPE;
    NVIC_EnableIRQ(ADC_IRQn);

    //set compare mode to give interrupt when converted data is lower than threshold
    ADC->ADC_EMR &= ~(1 << ADC_EMR_CMPMODE_Pos);
    //compare on channel 6 / A6
    ADC->ADC_EMR |= (6 << ADC_EMR_CMPSEL_Pos);

    //setting low threshold
    ADC->ADC_CWR |= ADC_LOWER_TRESHHOLD; //THIS VALUE MUST DE TESTED AND CHANGED!!!!
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