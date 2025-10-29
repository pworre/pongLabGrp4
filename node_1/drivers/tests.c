#include "tests.h"


void send_can_msg_test(uint8_t decode){
    CAN_MESSAGE message;
    message.data[0] = 6;
    message.data[1] = 7;
    message.size = 2;
    message.id = 9;

    uint8_t error_reg = 0;
    uint8_t status_reg = 0;
    uint8_t i = 0;
    
    while(1){
        can_send_msg(message);
        _delay_ms(100);
        
        printf("\r\nSEND: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", message.id, message.size message.data[0], message.data[1]); 

        status_reg = CAN_CTRL_read(MCP_CANSTAT);
        error_reg = CAN_CTRL_read(MCP_EFLG);
        if (decode){
            printf("Status reg:     ");
            print_byte(status_reg);
            printf("Error reg:      ");
            print_bytee(error_reg);
        }
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
        message = can_recive_msg(0);
        _delay_ms(100);
        
        printf("\r\nRECIVE: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", message.id, message.size message.data[0], message.data[1]); 

        status_reg = CAN_CTRL_read(MCP_CANSTAT);
        error_reg = CAN_CTRL_read(MCP_EFLG);
        if (decode){
            printf("Status reg:     ");
            print_byte(status_reg);
            printf("Error reg:      ");
            print_bytee(error_reg);
        }
        i++;
        _delay_ms(1000);
    }
}

void print_byte(uint8_t number){
    for (uint8_t k = 8; k >= 1; k--) {
            printf("%d", (number >> (k-1)) & 1);
        } printf("\r\n");
}

void print_CNx_registers(void){
    uint8_t CNF1_data = CAN_CTRL_read(MCP_CNF1); 
    uint8_t CNF2_data = CAN_CTRL_read(MCP_CNF2); 
    uint8_t CNF3_data = CAN_CTRL_read(MCP_CNF3); 
    printf("CNF1 : ");
    print_byte(CNF1_data);
    printf("CNF2 : ");
    print_byte(CNF2_data);
    printf("CNF3 : ");
    print_byte(CNF3_data);
}