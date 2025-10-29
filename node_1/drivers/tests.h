#ifndef TESTS_H
#define TESTS_H

#include "CAN_CTRL.h"
#include <util/delay.h>

void send_can_msg_test(uint8_t decode); //sends a constant can_msg every second, decode != 0 prints error and status regs for MCP2515
void recive_can_msg_test(uint8_t decode); //revices a can_msg and prints it, same behavior for decode
void print_byte(uint8_t number); //prints a byte: 01101100
void print_CNx_registers(void) //prints the CNx buffers used to set the bit timing
    

#endif