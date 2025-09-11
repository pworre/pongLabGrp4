#include <stdint.h>
#include <stdio.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"

#define TEST_PIN 1

int main(){
    USART0_Init(MYUBRR);
    fdevopen(USART0_send, USART0_read);
    unsigned char c; 
    printf("Hei studass vi er ferdig med oving 1 \r\n");
    while (1){
        //printf("Hei hei\r\n");
        //char letter = USART0_read();
        //printf("%c\n", letter);
        //USART0_send(letter);
        USART0_send(USART0_read());

    }
    return 0;
}
