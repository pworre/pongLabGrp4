#include "tests.h"

void send_can_msg_test(uint32_t decode){
    CAN_MESSAGE message;
    message.data[0] = 6;
    message.data[1] = 7;
    message.data_length = 2;
    message.id = 9;

    uint32_t status_reg = 0;
    uint32_t i = 0;
    
    while(1){
        if (can_send(&message, 1) == 1){
            printf("ikke sender\r\n");
        } else {
            printf("SENDER :)");
        }
        
        printf("\r\nSEND: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", message.id, message.data_length, message.data[0], message.data[1]); 

        if (decode){
            status_reg = CAN0->CAN_SR;
            printf("Satus reg = ");
            print_reg(status_reg);
        }
        i++;
        //delay
        for(volatile uint32_t i = 0; i < 7000000; i++){
            __asm__("nop");
        }
    }
}

void recive_can_msg_test(uint32_t decode){
    CAN_MESSAGE message;
    message.data[0] = 0;
    message.data[1] = 0;
    message.data_length = 2;
    message.id = 0;

    uint32_t status_reg = 0;
    uint32_t i = 0;
    
    while(1){
        if (can_receive(&message, 2) == 1){
            printf("ikke sender\r\n");
        } else {
            printf("SENDER :)");
        }
        
        printf("\r\nRecive: Iteration %u\r\n", i);
        printf("msg_id = %u    msg_size = %u    msg_data[0] = %u    msg_data[1] = %u\r\n", message.id, message.data_length, message.data[0], message.data[1]); 

        if (decode){
            status_reg = CAN0->CAN_SR;
            printf("Satus reg = ");
            print_reg(status_reg);
        }
        i++;
        //delay
        for(volatile uint32_t i = 0; i < 7000000; i++){
            __asm__("nop");
        }
    }
}

void print_reg(uint32_t number){
    for (uint32_t p = 32; p >= 1; p--){
            printf("%d", (number >> (p-1)) & 1);
        }
    printf("\r\n");
}