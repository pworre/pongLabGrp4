#include "tests.h"


void send_can_msg_test(uint8_t decode){
    CAN_MESSAGE message;
    message.data[0] = 10;
    message.data[1] = 11;
    message.size = 2;
    message.id = 9;

    uint8_t error_reg = 0;
    uint8_t status_reg = 0;
    uint8_t i = 0;
    
    while(1){
        can_send_msg(message);
        _delay_ms(100);
        
        printf("\r\nSEND: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", message.id, message.size, message.data[0], message.data[1]); 

        // status_reg = CAN_CTRL_read(MCP_CANSTAT);
        // error_reg = CAN_CTRL_read(MCP_EFLG);
        // if (decode){
        //     printf("Status reg:     ");
        //     print_byte(status_reg);
        //     printf("Error reg:      ");
        //     print_byte(error_reg);
        // }
        i++;
        _delay_ms(1000);
    }
}

void recive_can_msg_test(uint8_t decode){
    CAN_MESSAGE message;
    message.data[0] = 0;
    message.data[1] = 0;
    message.size = 2;
    message.id = 0;

    uint8_t error_reg = 0;
    uint8_t status_reg = 0;
    uint8_t i = 0;

    while (1){
        //can_recive_msg(&message, 1);
        //_delay_ms(100);
        
        printf("\r\nRX: itr %u\r\n", i);
        //printf("ID = %u    SIZE = %u    DATA[0] = %u    DATA[1] = %u\r\n", message.id, message.size, message.data[0], message.data[1]); 
        printf("ID = %u    SIZE = %u    DATA[0] = %u    DATA[1] = %u\r\n", msg_global.id, msg_global.size, msg_global.data[0], msg_global.data[1]); 

        // status_reg = CAN_CTRL_read(MCP_CANSTAT);
        // error_reg = CAN_CTRL_read(MCP_EFLG);
        // if (decode){
        //     printf("Status reg:     ");
        //     print_byte(status_reg);
        //     printf("Error reg:      ");
        //     print_byte(error_reg);
        // }
        i++;
        _delay_ms(1000);
    }
}

void send_joystick_with_can(void){
    CAN_MESSAGE message;
    message.data[0] = 0;
    message.data[1] = 0;
    message.data[2] = 0;
    message.data[3] = 0;
    message.data[4] = 0;
    message.data[5] = 0;
    message.size = 6;
    message.id = 0x0f;

    uint8_t i = 0;

    while (1)
    {   
        get_io_board_values();
        message.data[0] = scaled_values[0];
        message.data[1] = scaled_values[1];
        message.data[2] = buttons.right;
        message.data[3] = buttons.left;
        message.data[4] = buttons.nav;

        can_send_msg(message);
        _delay_ms(100);
        
        printf("\r\nSEND JOYSTICK: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    X = %d    Y = %d\r\n", message.id, message.size, message.data[0], message.data[1]); 
        i++;
        _delay_ms(100);
    }
}


void print_byte(uint8_t number){
    for (uint8_t k = 8; k >= 1; k--) {
            printf("%d", (number >> (k-1)) & 1);
        } printf("\r\n");
}

void print_CNx_registers(void){
    uint8_t CNF1_data = CAN_CTRL_read(MCP_CNF1); 
    // uint8_t CNF2_data = CAN_CTRL_read(MCP_CNF2); 
    // uint8_t CNF3_data = CAN_CTRL_read(MCP_CNF3); 
    // printf("CNF1 : ");
    // print_byte(CNF1_data);
    // printf("CNF2 : ");
    // print_byte(CNF2_data);
    // printf("CNF3 : ");
    // print_byte(CNF3_data);
}