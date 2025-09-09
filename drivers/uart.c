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

void USART0_Init(unsigned int ubrr)
{
    /* Set baud rate */
    //USART0->UBRR0H = (unsigned char)(ubrr>>8);
    USART0->UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    USART0->UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    USART0->UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void USART0_send(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(USART0->UCSR0A & (1 << UDRE0)));
    /* Put data into buffer, sends the data */
    USART0->UDR0 = data;
}

unsigned char USART0_Read(void)
{
    /* Wait for data to be received */
    while (!(USART0->UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */
    return USART0->UDR0;
}

void USART0_send_str(char * str){
    //char * letter_ptr = *str;
    while(*str != '\0'){
        USART0_send(*str);
        str++;
    }
}