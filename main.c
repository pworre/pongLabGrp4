#include <stdint.h>
#include <drivers/gpio.h>
#include <drivers/clock.h>

#define TEST_PIN 1

void testAtmega(){
    // Sets PIN as output
    GPIO->DDRA = (1 << TEST_PIN);

    while(1){

        if ((GPIO->PORTA == (1 << TEST_PIN)) && (CLOCK->TCNT3L == 0xFF)){
            GPIO->PORTA = 0;
        } if ((GPIO->PORTA == (1 << TEST_PIN)) && (CLOCK->TCNT3L == 0xFF)){
            GPIO->PORTA = (1 << TEST_PIN);
        }
    }
}

int main(){

    testAtmega();
    return 0;
}