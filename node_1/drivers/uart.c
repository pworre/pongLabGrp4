#include "uart.h"
#include "gpio.h"
#include "uart.h"

#define RXEN0 4
#define TXEN0 3
#define URSEL0 7
#define USBS0 3
#define UCSZ00 1
#define UDRE0 5
#define RXC0 7

void USART0_Init(unsigned int ubrr){
    //Baud rate 
    UCSR0C = (0<<URSEL0)|(unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    //Enable receiver and transmitter 
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    //Frame format: 8data, 2stop bit
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}


void USART0_send(unsigned char data){
    while (!(UCSR0A & (1 << UDRE0)));
    if (data == '\n'){
        UDR0 = '\r';
        while (!(UCSR0A & (1 << UDRE0)));
    }
    UDR0 = data;
}

unsigned char USART0_read(void){
    while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
}
