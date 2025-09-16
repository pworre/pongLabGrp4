#include "io_board.h"

// CLK Frequency 0.5 -> 5.0 MHZ
// Frekvens: 4.9152 MHz / (2 * (1 + 1)) = 1,23 MHz 
#define F_CLK 4915200UL
#define PRESCALER 1
#define freq 4

void io_board_init(){
    // Setter PD5 som output
    DDRD |= (1 << PD5);

    // CTC modus - Resettes når OC1A == OCR1A
    // WGMn3:0 = 4 --> CTC
    TCCR1B &= ~(0b111 << WGM10);
    TCCR1B |= (1 << WGM12);

    // Toggle OC1A når like
    TCCR1A |= (1 << COM1A0);

    // Setter Compare Register til 2
    OCR1AH = 0;
    OCR1AL = 1;

    // Enable source clock uten prescaler
    TCCR1B &= ~(0b111);
    TCCR1B |= (1 << CS10);
}