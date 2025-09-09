#include <stdint.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"

#define TEST_PIN 1

int main(){
    volatile uint32_t i = 0;
    GPIO ->DDRA = (1<<TEST_PIN);
    while (1){

        if (i % 30000 == 0){
            GPIO->PORTA ^= (1<<TEST_PIN);
        }
        i += 1;
    }

    return 0;
}