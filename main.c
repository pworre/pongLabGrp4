#include <stdint.h>
#include "drivers/gpio.h"
#include "drivers/clock.h"
#include "drivers/uart.h"

#define TEST_PIN 1

int main(){
    USART0_Init(MYUBRR);
    char letter = 'a';

    while (1){
        //letter = USART0_Read();
        //if (letter == 'a'){
            //USART0_send_str("abc");
        //}
        if (USART0_Read() == 'a'){
            USART0_send('a');
        }
    }
    return 0;
}