#ifndef ADC_H
#define ADC_H

#define ADC_LOWER_TRESHHOLD 3600 //2500 //change to make it make sence
#define ADC_HIGER_TRESHHOLd 4000 

#include "sam.h"

extern uint32_t goals;

void adc_init_freerun(void);


#endif