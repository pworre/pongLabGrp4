#ifndef PWM_H
#define PWM_H

#include "sam.h"

// Period
#define CPRD 13125
#define dutycycle_lower_bound 657
#define dutycycle_upper_bound 1312
#define dutycycle_middle 984


void pwm_init(void);
void pwm_set_dutycycle(uint32_t dutycycle);


#endif