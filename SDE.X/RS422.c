#include <xc.h>
#include "RS422.h"

void RS422_Init(void){
    
    //Setup UART pins
    TRISBbits.TRISB5 = OUTPUT;
    ANSELBbits.ANSB5 = FALSE;
    TRISBbits.TRISB2 = INPUT;
    ANSELBbits.ANSB2 = FALSE;
    APFCON0bits.RXDTSEL = TRUE; //UART RX on B2
    APFCON1bits.TXCKSEL = TRUE; //UART RX on B5
    
    //Setup transmitter
    TXSTAbits.TXEN = TRUE;
    BAUDCONbits.BRG16 = TRUE;
    TXSTAbits.BRGH = TRUE;
    SPBRGH = 0;
    SPBRGL = 68; //Should result in 115942 baud

    //Setup receiver
    RCSTAbits.CREN = TRUE; //Enable receiver

    //Setup interrupts
#ifndef UNBUFFERED_SER
    PIE1bits.RCIE = TRUE;
    PIE1bits.TXIE = TRUE;
#endif
    
    //Turn on port
    RCSTAbits.SPEN = TRUE;
}

void RS422_TxByte(unsigned char theByte){
#ifdef UNBUFFERED_SER
    TXREG = theByte;
    asm("NOP");
    while(!PIR1bits.TXIF){
        
    }
#else
    if(TXBUF_FREE > 0){
        //There's room in the buffer
        txbuf[txbufwrite++] = theByte;
    }
    else{
        commErrors.txBuffOvf++;
    }
#endif
}

//void RS422_SendBytes(unsigned char *buf, unsigned char count){
//    unsigned char i;
//    for(i=0; i<count; i++){
//        RS422_TxByte(buf[i]);
//    }
//}

inline void RS422_StartTx(void){
#ifndef UNBUFFERED_SER
    //Check if we're already transmitting
    if(PIE1bits.TXIE){
        return;
    }
    
    //Check if there's data in the buffer
    if(txbufread == txbufwrite){
        return;
    }
    
    TXREG = txbuf[txbufread++];
    
    if(txbufread != txbufwrite){
        PIE1bits.TXIE = TRUE;
    }
    
#endif
}

inline void RS422TXISR(void){
#ifndef UNBUFFERED_SER
    //Transmit the next character
    TXREG = txbuf[txbufread++];
    
    //If there's no more data in the buffer, disable interrupt
    if(txbufread == txbufwrite){
        PIE1bits.TXIE = FALSE;
    }
#endif
}

inline void RS422RXISR(void){
    if(RXBUF_FREE > 0){
        //Transfer into buffer
        rxbuf[rxbufwrite++] = RCREG;
    }
    else{
        commErrors.rxBuffOvf++;
    }
    
}