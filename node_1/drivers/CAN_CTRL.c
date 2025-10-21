#include "CAN_CTRL.h"

// Global variable:     STATUS OF TRANSMIT REGISTERS
uint8_t tx_reg0_ready = 1;
uint8_t tx_reg1_ready = 1;
uint8_t tx_reg2_ready = 1;

CAN_MESSAGE msg_global;

#define fosc 16000000 // Hz
#define bitrate 500000 // bit/s
#define TQ_per_bit 16
#define TQ_time (1/(bitrate * 16))
#define BRP 1
#define synchSeg 1
#define propSeg 3
#define phase1 6
#define phase2 4
#define SJW (16 - (synchSeg + propSeg + phase1 + phase2))

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



    // Set "low level" INT0 for Interrupt
    DDRD &= ~(1 << PD2);                        // Input
    PORTD |= (1 << PD2);                        // Intern pull-up
    //MCUCR &= ~((1 << ISC00) | (1 << ISC01));    // Low-level
    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);
    GICR |= (1 << INT0);                        // Activate INT0
    sei();

    CAN_CTRL_write(MCP_CANINTE, 0b10111111); // All, except wake-up

    //-------------------BIT TIMING OF CAN-BUS-------------------
    // BAUDRATE
    //  -   Bit rate = 500 kbits/sek
    //  -   TQ per bit = 16
    //  -   TQ time = 1 / bitrate / TQ per bit = 
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

    uint8_t config1_val = ((SJW << SJW0) | (BRP - 1));
    CAN_CTRL_write(MCP_CNF1, config1_val);

    uint8_t config2_val = ((1 << BTLMODE) | ((phase1 - 1) << PHSEG10) | ((propSeg - 1) << PRSEG0));
    CAN_CTRL_write(MCP_CNF2, config2_val);

    uint8_t config3_val = ((phase2 - 1) << PHSEG20);
    CAN_CTRL_write(MCP_CNF3, config3_val);

        //use loopback mode
    //CAN_CTRL_bit_modify(MCP_CANCTRL, 0b11100000, 0b00000000); 
    CAN_CTRL_write(MCP_CANCTRL, 0);


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

ISR(INT0_vect){
    uint8_t can_int_reg = CAN_CTRL_read(CANINTF);
    for (uint8_t bit = 0; bit < 8; bit++) {
        printf("%d", ((can_int_reg >> bit) & 1));
    } printf("\r\n");

    
    if ((can_int_reg & (1 << RX0IF)) != 0){         // Receive buffer 0 full
        //CAN_CTRL_write(CANINTF, 0);
        printf("Receive buffer 0 full!\r\n");

        msg_global = can_recive_msg(0);
        CAN_CTRL_bit_modify(CANINTF, (1 << RX0IF), 0);
    } else if ((can_int_reg & (1 << RX1IF)) != 0){  // Receive buffer 1 full
        printf("Receive buffer 1 full!\r\n");
        
        msg_global = can_recive_msg(1);
        CAN_CTRL_bit_modify(CANINTF, (1 << RX1IF), 0);
    } else if ((can_int_reg & (1 << TX0IF)) != 0){  // Transmit buffer 0 empty
        // Clear interrupt
        printf("Transmit buffer 0 empty!\r\n");
        
        tx_reg0_ready = 1;
        CAN_CTRL_bit_modify(CANINTF, (1 << TX0IF), 0);
    } else if ((can_int_reg & (1 << TX1IF)) != 0){  // Transmit buffer 1 empty
        // Clear interrupt
        printf("Transmit buffer 1 empty!\r\n");
        
        tx_reg1_ready = 1;
        CAN_CTRL_bit_modify(CANINTF, (1 << TX1IF), 0);
    } else if ((can_int_reg & (1 << TX2IF)) != 0){  // Transmit buffer 2 empty
        // Clear interrupt
        printf("Transmit buffer 2 empty!\r\n");
        
        tx_reg2_ready = 1;
        CAN_CTRL_bit_modify(CANINTF, (1 << TX2IF), 0);
    } else if ((can_int_reg & (1 << ERRIF)) != 0){  // Error
        // Clear interrupt
        
        // Read error flag register
        uint8_t error_register = CAN_CTRL_read(EFLG);
        if ((error_register & (1 << EWARN))){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << EWARN), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (EWARN: TEC or REC >= 96)");
        } else if ((error_register & (1 << RXWAR)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << RXWAR), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (REC >= 96)");
        } else if ((error_register & (1 << TXWAR)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << TXWAR), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (TEC >= 96)");
        } else if ((error_register & (1 << RXEP)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << RXEP), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (REC >= 128)");
        } else if ((error_register & (1 << TXEP)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << TXEP), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (REC >= 128)");
        } else if ((error_register & (1 << TXBO)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << TXBO), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (REC = 255) // BUS-OFF");
        } else if ((error_register & (1 << RX0OWR)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << RX0OWR), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (Receive buffer 0 overflow");
        } else if ((error_register & (1 << RX1OWR)) != 0){
            // Clear interrupt
            CAN_CTRL_bit_modify(EFLG, (1 << RX1OWR), 0);
            
            //printf("CAN-BUS INTERRUPT:  ERROR (Receive buffer 1 overflow");
        } else {
            //printf("CAN-BUS INTERRUPT:  ERROR");
        }
        CAN_CTRL_bit_modify(CANINTF, (1 << ERRIF), 0);
    } else if ((can_int_reg & (1 << WAKIF)) != 0){  // Wake-up
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, (1 << WAKIF), 0);
        
        // NOT IMPLEMENTED
    } else if ((can_int_reg & (1 << MERRF)) != 0){  // Message
        // Clear interrupt
        CAN_CTRL_bit_modify(CANINTF, (1 << MERRF), 0);
        
        //printf("CAN-BUS INTERRUPT:  MESSAGE FAULT");
    }
}

/*
void can_send_msg(CAN_MESSAGE can_msg){
    //this sends a msg from buffer 0

    CAN_CTRL_bit_modify(CANINTE, TX0IF, (1 << TX0IF));
    //set id i id high og low register
    CAN_CTRL_write(TXB0SIDH, (can_msg.id & 0x00ff));
    CAN_CTRL_write(TXB0SIDL, (can_msg.id & 0x0f00 >> 8));

    // set data lengt
    CAN_CTRL_write(TXB0DLC, can_msg.size);

    // set data
    for (uint8_t i = 0; i < can_msg.size; i++){
        CAN_CTRL_write(TXB0D0 + i, can_msg.data[i]);
    }
    //request to send buffer 0
    CAN_CTRL_RTS(0b001)
    
} */

void can_send_msg(CAN_MESSAGE can_msg){
    //this sends a msg from buffer 0
    //set id i id high og low register
    uint8_t idH = (can_msg.id >> 3);
    uint8_t idL = ((can_msg.id & 0x07) << 5);
    uint8_t id8 = 0;
    uint8_t id0 = 0;

    SPI_MasterTransmit(MCP_LOAD_TX0, CAN);
    SPI_MasterTransmit(idH, CAN);
    SPI_MasterTransmit(idL, CAN);
    //SPI_MasterTransmit(id8, CAN);
    //SPI_MasterTransmit(id0, CAN);
    SPI_MasterTransmit(can_msg.size & 0x0f, CAN);

    // set data
    for (uint8_t i = 0; i < can_msg.size; i++){
        SPI_MasterTransmit(can_msg.data[i], CAN);
    }
    SPI_slave_deselect();
    SPI_MasterTransmit(MCP_RTS_TX0, CAN);
    SPI_slave_deselect();
}

/*
CAN_MESSAGE can_recive_msg(uint8_t buffer_nr){
    CAN_MESSAGE msg = {};
    uint8_t buffer_offset = 0;
    if (buffer_nr == 1){
        buffer_offset = 0b10000;
    }
    msg.id = CAN_CTRL_read(RXB0SIDL + buffer_offset) + ((CAN_CTRL_read(RXB0SIDH + buffer_offset) & 0x0f) << 8);

    msg.size = CAN_CTRL_read(RXB0DLC + buffer_offset);
    if (msg.size > 8){
        msg.size = 8;
    }

    for (uint8_t i = 0; i < msg.size; i++){
        msg.data[i] = CAN_CTRL_read(RXB0D0 + buffer_offset + i);
    }
    //sett the CANINTF.RX0IF = 0 to signal that the msg is fetched
    CAN_CTRL_bit_modify(CANINTF, 0b00000001, 0);


    return msg;
} */

CAN_MESSAGE can_recive_msg(uint8_t buffer_nr){
    CAN_MESSAGE msg = {};

    if (buffer_nr == 1){
        SPI_MasterTransmit(MCP_READ_RX0, CAN);

        uint8_t idH = SPI_read(CAN);
        uint8_t idL = SPI_read(CAN);
        //SPI_read(CAN);
        //SPI_read(CAN);
        msg.size = (SPI_read(CAN) & 0x0f);

        msg.id = (idH << 3) | (idL >> 5);

        for (uint8_t i = 0; i < msg.size; i++){
            msg.data[i] = SPI_read(CAN);
        }
        SPI_slave_deselect();
        return msg;
    } else if (buffer_nr == 2){
        SPI_MasterTransmit(MCP_READ_RX1, CAN);

        uint8_t idH = SPI_read(CAN);
        uint8_t idL = SPI_read(CAN);
        //SPI_read(CAN);
        //SPI_read(CAN);
        msg.size = (SPI_read(CAN) & 0x0f);

        msg.id = (idH << 3) | (idL >> 5);

        for (uint8_t i = 0; i < msg.size; i++){
            msg.data[i] = SPI_read(CAN);
        }
        SPI_slave_deselect();
        return msg;
    } else {
        printf("CAN_RECEIVE_MSG:        No RX-Buffers is currently full");
    }

}