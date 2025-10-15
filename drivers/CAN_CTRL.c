#include "CAN_CTRL.h"

// Global variable:     STATUS OF TRANSMIT REGISTERS
uint8_t tx_reg0_ready = 1;
uint8_t tx_reg1_ready = 1;
uint8_t tx_reg2_ready = 1;

void test_CAN_CTRL(void) {
    CAN_CTRL_init();
    CAN_CTRL_write(0b00000000, 0b00000011);
    uint8_t data = CAN_CTRL_read(0b00000000);
    printf("Value: %u\r\n", (unsigned int)data);
}


void CAN_CTRL_init(void){
    CAN_CTRL_reset();

    //no filters on Reseve buffer 0
    CAN_CTRL_write(MCP_RXB0CTRL, 0b01100000);

    //use loopback mode
    CAN_CTRL_bit_modify(MCP_CANCTRL, 0b11100000, 0b01000000); 

    // Set "low level" INT0 for Interrupt
    DDRD &= ~(1 << PD2);                        // Input
    MCUCR &= ~((1 << ISC00) | (1 << ISC01));    // Low-level
    GICR |= (1 << INT0);                        // Activate INT0
    sei();

    CAN_CTRL_write(MCP_CANINTE, 0b10111111); // All, except wake-up

    //-------------------BIT TIMING OF CAN-BUS-------------------
    // BAUDRATE
    //  -   Bit rate = 500 kbits/sek
    //  -   TQ per bit = 8
    //  -   TQ time = 1 / bitrate / TQ per bit
    //  -   fosc = 16 MHz
    //  -   baudrate_prescaler = TQ time * fosc / 2 - 1 = 1 

    // TIMING       (PSn = Phase segment n)
    //  -   PropagationSegment + PS1 >= PS2
    //  -   PropagationSegment + PS1 >= Tdelay, Tdelay (1->2 TQ)

    // 75% sampling point (8 TQ : Time Quantum)
    //  -   1 TQ for sychronize segment
    //  -   1 TQ for propagation segment
    //  -   4 TQ for PS1
    //  -   2 TQ for PS2
    // SJW bits = 0 to synch jump width length = 1 x TQ
    uint8_t baud_prescaler = 1;

    uint8_t config1_val = (SJW1 | baud_prescaler);
    CAN_CTRL_write(MCP_CNF1, config1_val);

    uint8_t config2_val = (BTLMODE | SAM | 0b00100000 | 1);
    CAN_CTRL_write(MCP_CNF2, config2_val);

    uint8_t config3_val = (SOF | WAKFIL | 2);
    CAN_CTRL_write(MCP_CNF3, config3_val);


}

void CAN_CTRL_reset(void){
    SPI_MasterTransmit(MCP_RESET, CAN); //reset cmd
    SPI_slave_deselect();
}


void CAN_CTRL_write(uint8_t address, uint8_t data){
    SPI_MasterTransmit(MCP_WRITE, CAN); //write cmd
    SPI_MasterTransmit(address, CAN);
    SPI_MasterTransmit(data, CAN);
    SPI_slave_deselect();
}


uint8_t CAN_CTRL_read(uint8_t address){
    SPI_MasterTransmit(MCP_READ, CAN); //read cmd
    SPI_MasterTransmit(address, CAN);
    uint8_t data = SPI_read(CAN);
    SPI_slave_deselect();
    return data;
}


void CAN_CTRL_RTS(uint8_t buffer_nr){
    uint8_t number = buffer_nr & 0b00000111;
    uint8_t command = 0b10000000 | number;
    SPI_MasterTransmit(command, CAN);
    SPI_slave_deselect();
}


uint8_t CAN_CTRL_read_status(void){
    SPI_MasterTransmit(MCP_READ_STATUS, CAN);
    uint8_t data = SPI_read(CAN);
    SPI_slave_deselect();
    return data;
}


void CAN_CTRL_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
    SPI_MasterTransmit(MCP_BITMOD, CAN);
    SPI_MasterTransmit(mask, CAN);
    SPI_MasterTransmit(data, CAN);
    SPI_slave_deselect();
}

ISR(INTO_vect){
    uint8_t can_int_reg = CAN_CTRL_read(CANINTF);

    
    if ((can_int_reg & (1 << RX0IF)) != 0){         // Receive buffer 0 full
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, RX0IF, 0);

        CAN_CTRL_read(RXB0D0);
    } else if ((can_int_reg & (1 << RX1IF)) == 1){  // Receive buffer 1 full
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, RX1IF, 0);

        CAN_CTRL_read(RXB1D0);
    } else if ((can_int_reg & (1 << TX0IF)) == 1){  // Transmit buffer 0 empty
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, TX0IF, 0);

        tx_reg0_ready = 1;
    } else if ((can_int_reg & (1 << TX1IF)) == 1){  // Transmit buffer 1 empty
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, TX1IF, 0);

        tx_reg1_ready = 1;
    } else if ((can_int_reg & (1 << TX2IF)) == 1){  // Transmit buffer 2 empty
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, TX2IF, 0);

        tx_reg2_ready = 1;
    } else if ((can_int_reg & (1 << ERRIF)) == 1){  // Error
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, ERRIF, 0);

        // Read error flag register
        uint8_t error_register = CAN_CTRL_read(EFLG);
        if ((error_register & (1 << EWARN))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, EWARN, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (EWARN: TEC or REC >= 96)");
        } else if ((error_register & (1 << RXWAR))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, RXWAR, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (REC >= 96)");
        } else if ((error_register & (1 << TXWAR))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, TXWAR, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (TEC >= 96)");
        } else if ((error_register & (1 << RXEP))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, RXEP, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (REC >= 128)");
        } else if ((error_register & (1 << TXEP))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, TXEP, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (REC >= 128)");
        } else if ((error_register & (1 << TXBO))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, TXBO, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (REC = 255) // BUS-OFF");
        } else if ((error_register & (1 << RX0OWR))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, RX0OWR, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (Receive buffer 0 overflow");
        } else if ((error_register & (1 << RX1OWR))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, RX1OWR, 0);

            printf("CAN-BUS INTERRUPT:  ERROR (Receive buffer 1 overflow");
        }


        printf("CAN-BUS INTERRUPT:  ERROR");
    } else if ((can_int_reg & (1 << WAKIF)) == 1){  // Wake-up
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, WAKIF, 0);

        // NOT IMPLEMENTED
    } else if ((can_int_reg & (1 << MERRF)) == 1){  // Message
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, MERRF, 0);

        printf("CAN-BUS INTERRUPT:  MESSAGE FAULT");
    }
}