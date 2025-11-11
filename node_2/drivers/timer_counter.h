#ifndef TIMER_COUNTER_H
#define TIMER_COUNTER_H

typedef struct pid_controller PID_CONTROLLER;

#include "sam.h"
#include "pid_controller.h"

extern uint32_t score;
extern PID_CONTROLLER pid_ctrl;

void timer_counter_init(uint32_t tc_channel, uint32_t period); //tc_channel is 0 for score and 1 for motor control, period is given as clocksycles 84M Hz



#endif