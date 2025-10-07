#include "spi.h"

void SPI_MasterInit(void)
{
    // Set MOSI and SCK output
    DDRB |= (1<<PB7)|(1<<PB5)|(1<<PB4)|(1<<PB3)|(1<<PB2)|(1<<PB1);
    DDRB &= ~(1 << PB6);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(uint8_t data, SLAVES slave)
{
    // Velg rett slave med rett SS til lav
    switch (slave) {
        case IO_BOARD:
            PORTB &= ~(1<<SS_IO_BOARD); //sett rett SS lav og de andre SS høy
            PORTB |= (1<<SS_OLED)|(1<<SS_ARDUINO);
            break;

        case OLED:
            PORTB &= ~(1<<SS_OLED);
            PORTB |= (1<<SS_IO_BOARD)|(1<<SS_ARDUINO);
            break;

        case ARDUINO:
            PORTB &= ~(1<<SS_ARDUINO);
            PORTB |= (1<<SS_OLED)|(1<<SS_IO_BOARD);
            break;

        default:
            //printf("Ukjent slave");
            break;
    }
    /* Start transmission */
    SPDR = data;

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
    _delay_us(10);
}

uint8_t SPI_read(SLAVES slave){
    SPI_MasterTransmit(0, slave);
    _delay_us(DELAY_COMMAND_DATA);
    return SPDR;
}