#ifndef TESTS_H
#define TESTS_H

#include "can_controller.h"
#include "sam.h"
#include "pwm.h"

void send_can_msg_test(uint32_t decode); //sends a constant can_msg every second, decode != 0 prints status reg
void recive_can_msg_test(uint32_t decode); //revices a can_msg and prints it, same behavior for decode, NB: uses rx as reg 1! so only 1 tx reg.
void controll_servo_with_buttons_test(void);
void control_servo_with_joystick_test(void);
void print_reg(uint32_t number); //prints a byte: 01101100

#endif