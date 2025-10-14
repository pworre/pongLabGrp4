#include "spi.h"
#include "MCP2515_registers.h"
#include "avr/interrupt.h"

#ifndef MCP2515_H
#define MCP2515_H

// Global variable:     STATUS OF TRANSMIT REGISTERS
uint8_t tx_reg0_ready = 1;
uint8_t tx_reg1_ready = 1;
uint8_t tx_reg2_ready = 1;

void CAN_CTRL_init(void);
void CAN_CTRL_reset(void); //resets the CAN controller and lets it be ready for init
void CAN_CTRL_write(uint8_t address, uint8_t data); //writes data to the address in the MCP2515
uint8_t CAN_CTRL_read(uint8_t address); //read the data from the address in the MCP2515
void CAN_CTRL_RTS(uint8_t buffer_nr); //request the buffer of number buffer_nr to be sendt, 0b100 = TXB2, 0b010 = TXB1, 0b001 = TXB0
uint8_t CAN_CTRL_read_status(void); //reads the status bits
void CAN_CTRL_bit_modify(uint8_t address, uint8_t mask, uint8_t data); //modifyes the data od address with using a mask

void test_CAN_CTRL(void);

#endif