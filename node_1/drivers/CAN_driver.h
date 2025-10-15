#include "CAN_CTRL.h"
#include "MCP2515_registers.h"

#ifndef CAN_DRIVER_H
#define CAN_DDRIVER_H

uint8_t tx_reg0_ready;
uint8_t tx_reg1_ready;
uint8_t tx_reg2_ready;
uint8_t rx_reg0_full;
uint8_t rx_reg1_full;

typedef struct CAN_MESSAGE{
    uint16_t id;
    uint8_t size;
    uint8_t data[8];
} CAN_MESSAGE;

void can_send_msg(CAN_MESSAGE can_msg);
CAN_MESSAGE can_recive_msg(uint8_t buffer_nr);

#endif