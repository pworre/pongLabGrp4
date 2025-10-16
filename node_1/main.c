#define F_CPU 4915200UL
#include <stdint.h>
#include <stdio.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"
#include "drivers/sram.h"
#include "drivers/io_board.h"
#include "drivers/spi.h"
#include "drivers/oled.h"
#include "drivers/fonts.h"
#include "drivers/menu.h"
#include "drivers/CAN_CTRL.h"
#include <util/delay.h>
#include "drivers/CAN_driver.h"

int main(){

    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);

    sram_init();
    //SRAM_test();
    
    io_board_init();
    io_board_calibration();

    SPI_MasterInit();
    oled_init();

    CAN_CTRL_init();
    uint8_t i = 0;
    while (1){
        /*
        CAN_MESSAGE message;
        message.data[0] = i;
        message.id = i;
        message.size = 1;
        */

        CAN_MESSAGE message;
        message.id = 1;
        message.data[0] = 5;
        message.size = 1;

        
        //can_send_msg(message);
        CAN_CTRL_write(0b00110001, (message.id) >> 3);          // ID_low
        CAN_CTRL_write(0b00110010, (message.id & 0x07) << 5);   // ID_high
        CAN_CTRL_write(0b00110101, (message.size));
        CAN_CTRL_write(0b00110110, message.data[0]);

        _delay_ms(100);
        //while (! (CAN_CTRL_read(MCP_CANINTF) & (1 << RX0IF)));
        //CAN_MESSAGE new_msg = can_recive_msg(0);
        uint8_t idH = CAN_CTRL_read(0b00110001);
        uint8_t idL = CAN_CTRL_read(0b00110010);
        uint8_t id = ((idH << 3) | (idL >> 5));
        uint8_t size = CAN_CTRL_read(0b00110101);
        uint8_t data = CAN_CTRL_read(0b00110110);

        printf("OLD_msg_id = %u    OLD_msg_data = %u\r\n", message.id, message.data[0]); 
        printf("ID = %u     size = %u    data = %u\r\n", id, size, data); 
        //printf("NEW_msg_id = %u    NEW_msg_data = %u\r\n", new_msg.id, new_msg.data[0]); 

        _delay_ms(1);
        i++;
    }

    return 0;
}
