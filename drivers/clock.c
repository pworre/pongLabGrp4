#include <clock.h>

#define WGM12 3

void counter1_init(){
    COUNTER1->TCCR1B |= (1 << WGM12);    // CTC-mode
    COUNTER1->TCCR1B |= b101;
}