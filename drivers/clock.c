#include <clock.h>

#define WGM12 3
#define CS10 0 
#define CS12 2
#define OCIE1A 6

void counter1_init(){
    COUNTER1->TCCR1B |= (1 << WGM12);               // CTC-mode
    COUNTER1->TCCR1B |= (1 << CS10) | (1 << CS12);  // Prescaler 1024

    // Compare value to 4.9152 MHz / 1024 = 4800        --- With this compare value, we should hit compare after 1s
    COUNTER1->OCR1AL = 0b11000000;                  // Low-byte for compare
    COUNTER1->OCR1AH = 0b00010010;                   // High-byte for compare

    // Enable A-compare-interrupt
    COUNTER1->TIMSK = (1 << OCIE1A);
}