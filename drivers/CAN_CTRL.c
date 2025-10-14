#include "CAN_CTRL.h"


void CAN_CTRL_init(void){
    CAN_CTRL_reset();

}

void CAN_CTRL_reset(void){
    SPI_MasterTransmit(0b11000000, CAN); //reset cmd
    SPI_slave_deselect();
}


void CAN_CTRL_write(uint8_t address, uint8_t data){
    SPI_MasterTransmit(0b00000010, CAN); //write cmd
    SPI_MasterTransmit(address, CAN);
    SPI_MasterTransmit(data, CAN);
    SPI_slave_deselect();
}


uint8_t CAN_CTRL_read(uint8_t address){
    SPI_MasterTransmit(0b00000011, CAN); //read cmd
    SPI_MasterTransmit(address, CAN);
    uint8_t data = SPI_read(CAN);
    SPI_slave_deselect();
    return data;
}


void CAN_CTRL_RTS(uint8_t buffer_nr){
    uint8_t number = buffer_nr & 0b00000111;
    uint8_t command = 0b10000000 | number;
    SPI_MasterTransmit(command, CAN);
    SPI_slave_deselect();
}


uint8_t CAN_CTRL_read_status(void){
    SPI_MasterTransmit(0b10100000, CAN);
    uint8_t data = SPI_read(CAN);
    SPI_slave_deselect();
    return data;
}


void CAN_CTRL_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
    SPI_MasterTransmit(0b00000101, CAN);
    SPI_MasterTransmit(mask, CAN);
    SPI_MasterTransmit(data, CAN);
    SPI_slave_deselect();
}