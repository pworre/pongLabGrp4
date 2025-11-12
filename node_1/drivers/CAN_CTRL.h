#include "spi.h"
#include "MCP2515_registers.h"
#include "avr/interrupt.h"

#ifndef CAN_CTRL_H
#define CAN_CTRL_H

void CAN_CTRL_init(void);
void CAN_CTRL_reset(void); //resets the CAN controller and lets it be ready for init
void CAN_CTRL_write(uint8_t address, uint8_t data); //writes data to the address in the MCP2515
uint8_t CAN_CTRL_read(uint8_t address); //read the data from the address in the MCP2515
void CAN_CTRL_RTS(); //request TX0 to be sendt
uint8_t CAN_CTRL_read_status(void); //reads the status bits
void CAN_CTRL_bit_modify(uint8_t address, uint8_t mask, uint8_t data); //modifyes the data od address with using a mask

void test_CAN_CTRL(void);



typedef struct{
    uint16_t id;
    uint8_t size;
    int8_t data[8];
} CAN_MESSAGE;

void can_send_msg(CAN_MESSAGE can_msg);
int can_recive_msg(CAN_MESSAGE *msg, uint8_t buffer_nr);

CAN_MESSAGE msg_global;

#endif