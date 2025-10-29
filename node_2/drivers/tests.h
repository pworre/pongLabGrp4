#ifndef TESTS_H
#define TESTS_H

#include "can_controller.h"
#include "sam.h"

void send_can_msg_test(uint32_t decode); //sends a constant can_msg every second, decode != 0 prints status reg
void recive_can_msg_test(uint32_t decode); //revices a can_msg and prints it, same behavior for decode
void loop_back_mode_test(void); //loopback mode: sends and recives 2 bytes of data where data and id increases
void print_reg(uint32_t number); //prints a byte: 01101100

#endif