#include "io_board.h"
#include "stdio.h"

// CLK Frequency 0.5 -> 5.0 MHZ
// Frekvens: 4.9152 MHz / (2 * (1 + 1)) = 1,23 MHz 
#define F_CLK 4915200UL
#define PRESCALER 1
#define freq 4

// CHANNEL 1 // JOYSTICK X
// CHANNEL 2 // JOYSTICK Y
// CHANNEL

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


void ADC_read(void){
    volatile uint8_t* adc_adress = (uint8_t *) 0x1000;
    static uint8_t adc_data[4];
    
    //adc_adress[0] = 0xff;
    adc_adress[0] = 0;

    _delay_us(100);
    while(!(PIND & (1 << PD4))){
    }

    for (uint8_t i = 0; i<4; i++){
        adc_data[i] = adc_adress[i];
    }

    printf("%d\r\n", adc_data[1]);
}


/*
    void ADC_read(uint8_t chanel){
    
    DDRA = 0b11111111;
    PORTA = chanel;
    PORTC &= ~(1<<3);
    PORTD &= ~(1<<6);

    _delay_us(2);
    
    PORTD |= (1<<6);
    DDRA = 0;

    while(!PD4){
    }
    _delay_us(2);

    PORTD &= ~(1<<7);

    adc_data[0] = PORTA;

    PORTD |= (1<<7);
    PORTC |= (1<<3);
    _delay_us(2);
}*/
/*
uint8_t get_adc_data(uint8_t port){
    return adc_data[port];
}
*/