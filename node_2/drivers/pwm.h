#ifndef PWM_H
#define PWM_H

#include "sam.h"

void pwm_init(void);
void pwm_set_dutycycle(uint32_t dutycycle);


#endif