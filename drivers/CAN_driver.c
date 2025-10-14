#include "CAN_driver.h"


void can_send_msg(CAN_MESSAGE can_msg){
    //this sends a msg from buffer 0

    CAN_CTRL_bit_modify(CANINTE, TX0IF, (1 << TX0IF));
    //set id i id high og low register
    CAN_CTRL_write(TXB0SIDH, (can_msg.id & 0x00ff));
    CAN_CTRL_write(TXB0SIDL, (can_msg.id & 0x0f00 >> 8));

    // set data lengt
    CAN_CTRL_write(TXB0DLC, can_msg.size);

    // set data
    for (uint8_t i = 0; i < can_msg.size; i++){
        CAN_CTRL_write(TXB0D0 + i, can_msg.data[i]);
    }
    //request to send buffer 0
    CAN_CTRL_RTS(0b001);
}

CAN_MESSAGE can_recive_msg(uint8_t buffer_nr){
    CAN_MESSAGE msg = {};
    uint8_t buffer_offset = 0;
    if (buffer_nr == 1){
        buffer_offset = 0b10000;
    }
    msg.id = CAN_CTRL_read(RXB0SIDL + buffer_offset) + ((CAN_CTRL_read(RXB0SIDH + buffer_offset) & 0x0f) << 8);

    msg.size = CAN_CTRL_read(RXB0DLC + buffer_offset);
    if (msg.size > 8){
        msg.size = 8;
    }

    for (uint8_t i = 0; i < msg.size; i++){
        msg.data[i] = CAN_CTRL_read(RXB0D0 + buffer_offset + i);
    }
    //sett the CANINTF.RX0IF = 0 to signal that the msg is fetched
    CAN_CTRL_bit_modify(CANINTF, 0b00000001, 0);


    return msg;
}