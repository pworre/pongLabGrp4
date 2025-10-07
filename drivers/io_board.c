#include "io_board.h"
#include "stdio.h"

// CLK Frequency 0.5 -> 5.0 MHZ
// Frekvens: 4.9152 MHz / (2 * (1 + 1)) = 1,23 MHz 
#define F_CLK 4915200UL
#define PRESCALER 1
#define freq 4
#define btn_joystick_pin 2

// CHANNEL 0 // JOYSTICK X
// CHANNEL 1 // JOYSTICK Y
// CHANNEL 2 // TOUCH X
// CHANNEL 3 //

volatile uint8_t* adc_adress = (uint8_t *) 0x1000;
static uint8_t adc_data[4];
static int8_t scaled_values[4];
uint8_t joystick_x_calibration, joystick_y_calibration;

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
    TCCR1B &= ~(0b111);uint8_t buttons = 0;
    TCCR1B |= (1 << CS10);

    // Setup joystick button as input
    DDRE &= ~(1 << btn_joystick_pin);
    buttons = 0;
}


void ADC_read(void){

    
    //adc_adress[0] = 0xff;
    adc_adress[0] = 0;

    _delay_us(100);
    while(!(PIND & (1 << PD4))){
    }

    for (uint8_t i = 0; i<4; i++){
        adc_data[i] = adc_adress[i];
    }

    //printf("%d\r\n", adc_data[1]);
}

void ADC_print(void){
    get_io_board_values();
    printf("Joystick: %3d, %3d          Touch: %3d, %3d\r\n", scaled_values[0], scaled_values[1], scaled_values[2], scaled_values[3]);
}

void io_board_calibration(void){
    uint8_t full_scale = 100;
    ADC_read();
    joystick_x_calibration = adc_data[1];
    joystick_y_calibration = adc_data[0];
    
}

void get_io_board_values(void){
    uint8_t x_joystick_max = 250;
    uint8_t x_joystick_min = 77;
    uint8_t y_joystick_max = 255;
    uint8_t y_joystick_min = 67;
    uint8_t touch_max = 255;
    uint8_t touch_min = 0;

    ADC_read();
    
    //joystick x
    scaled_values[0] = ((adc_data[1] - joystick_x_calibration) * 100)/ ((x_joystick_max-x_joystick_min)/2);

    //joystick y
    scaled_values[1] = ((adc_data[0] - joystick_y_calibration) * 100)/ ((y_joystick_max-y_joystick_min)/2);

    //touch x
    scaled_values[2] = (((adc_data[3] - (touch_max-touch_min)/2)) * 100)/ ((touch_max-touch_min)/2) ;

    //touch y
    scaled_values[3] = (((adc_data[2] - (touch_max-touch_min)/2)) *100)/ ((touch_max-touch_min)/2);

    /*
    // Buttons
    if ((PINE & (1 << btn_joystick_pin)) == 0) {
        // Joystick button pressed
        printf("Joystick Button pressed \r");
        SPI_MasterTransmit((0x05), IO_BOARD);
        SPI_MasterTransmit(3, IO_BOARD);
        SPI_MasterTransmit(3, IO_BOARD);
    } else {
        // IKKE TRYKT
    }*/

    uint8_t buttons;
    // Buttons consists of 3 bytes: RIGHT, LEFT, NAV
    SPI_MasterTransmit((0x04), IO_BOARD);
    for (int byte_nr = 0; byte_nr < 3; byte_nr++){
        switch(byte_nr){
            case 0: // RIGHT
                buttons = SPI_read(0, IO_BOARD);
            case 1: // LEFT
                buttons = SPI_read(0, IO_BOARD);
            case 2: // NAV
                buttons = SPI_read(0, IO_BOARD);
                if ((buttons & (1 << 4)) == 1){
                    printf("NAV-knapp trykket");
                }
        }
        
    }
    
}

void get_io_board_directions(void){
    uint8_t x = scaled_values[0];
    uint8_t y = scaled_values[1];

    if (x < 20 && y < 20){
        joystick_dir = IDLE;
    }
    else if (x > y){
        if (x < 0){
            joystick_dir = DOWN;
        } 
        else{
            joystick_dir = UP;
        }
    }
    else{
        if (y < 0){
            joystick_dir =LEFT;
        }
        else{
            joystick_dir = RIGHT;
        }
    }
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