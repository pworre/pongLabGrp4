#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "sam.h"

// ENABLE/PWM PB12
// PHASE/DIR PC23
//PWN_OUT PB

typedef enum {
    RIGHT,
    LEFT
} DC_DIRECTIONS;

void motor_init(void);
void motor_setdir(DC_DIRECTIONS dir);

// Set power in percentage
void motor_setpower(uint32_t power);

#endif