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
    UCSR0C = (0<<URSEL0)|(unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}


void USART0_send(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char USART0_read(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */
    return UDR0;
}
