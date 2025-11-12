#include <stdint.h>
#include <avr/io.h>
#ifndef UART_H
#define UART_H

#define FOSC 4915200UL// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

/*
#define USART0 ((ATMEGA_UART_REG*)0x29)

typedef struct {
    volatile uint8_t UBRR0L;
    volatile uint8_t UCSR0B;
    volatile uint8_t UCSR0A;
    volatile uint8_t UDR0;
    volatile uint8_t RESERVED0[20];
    volatile uint8_t UCSR0C; //MÅ VELGES!!
    //volatile uint8_t UBRR0H; //MÅ VELGES!!
} ATMEGA_UART_REG;
*/

void USART0_Init(unsigned int ubrr);
void USART0_send(unsigned char data);
unsigned char USART0_read(void);

#endif