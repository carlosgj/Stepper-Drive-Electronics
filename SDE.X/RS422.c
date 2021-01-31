#include <xc.h>
#include "RS422.h"

void RS422_Init(void){
    
    //Setup UART pins
    TRISCbits.TRISC6 = OUTPUT;
    TRISCbits.TRISC7 = INPUT;
    U1RXPPS = 0b10111; //UART1 RX on C7
    RC6PPS = 0b010011; //C6 is UART1 TX
    
    U1CON0bits.TXEN = TRUE;
    ANSELCbits.ANSELC6 = FALSE;
    U1CON0bits.U1MODE = 0b0000; //8-bit async mode
    U1CON0bits.BRGS = 1; // FOSC/[4 (n+1)]
    U1BRGHbits.BRGH = (RS422_BRG_VAL >> 8);
    U1BRGLbits.BRGL = (RS422_BRG_VAL & 255);
    
    U1CON0bits.RXEN = TRUE;
    ANSELCbits.ANSELC7 = FALSE;
    PIE3bits.U1RXIE = TRUE;
    
    U1CON1bits.ON = TRUE;
    
    commErrors.frameBufOvf = 0;
    commErrors.msgBufOvf = 0;
    commErrors.rxBuffOvf = 0;
    commErrors.txBuffOvf = 0;
}

void RS422_TxByte(unsigned char theByte){
    U1TXB = theByte;
    while(!PIR3bits.U1TXIF){};
}

void RS422_SendBytes(unsigned char *buf, unsigned char count){
    unsigned char i;
    for(i=0; i<count; i++){
        RS422_TxByte(buf[i]);
    }
}

void RS422_StartTx(void){
    //Check if we're already transmitting
    if(PIE3bits.U1TXIE){
        return;
    }
    
    //Check if there's data in the buffer
    if(txbufread == txbufwrite){
        return;
    }
    
    //U1TXB = txbuf[txbufread++];
    
    if(txbufread != txbufwrite){
        PIE3bits.U1TXIE = TRUE;
    }
    
    
}

void __interrupt(irq(U1TX), low_priority) RS422TXISR(void){
    //Transmit the next character
    U1TXB = txbuf[txbufread++];
    
    //If there's no more data in the buffer, disable interrupt
    if(txbufread == txbufwrite){
        PIE3bits.U1TXIE = FALSE;
    }
}

void __interrupt(irq(U1RX), low_priority) RS422RXISR(void){
    //Transfer into buffer
    rxbuf[rxbufwrite++] = U1RXB;
}