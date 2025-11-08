#ifndef ADC_H
#define ADC_H

#define ADC_LOWER_TRESHHOLD 100 //2500 //change to make it make sence
#define ADC_HIGER_TRESHHOLd 1000 

#include "sam.h"

extern uint32_t goals;

void adc_init_freerun(void);


#endif