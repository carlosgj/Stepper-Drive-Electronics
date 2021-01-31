#include <xc.h>
#include "main.h"
#include <stdlib.h>

void main(void) {
    init();
    while(TRUE){
        CLRWDT();
        run();
    }
}

void init(void){
    INTDIS;
    //Setup oscillator
    //INTCON0bits.IPEN = TRUE; //Enable interrupt priority
    //OSCCON1bits.NDIV = 0; //Request no divider
    //OSCFRQ = 0b0111; //48 MHz
    
#ifdef LOOPOUT
    TRISAbits.TRISA7 = OUTPUT;
#endif
    INTEN;
    
    timerInit();
    __delay_ms(200);
    
    RS422_Init();
    HDLCInit();
    sendSUSEVR(SUS_INITIAL);
    sendSwVerEVR();
    
    SPIInit();
    
    EEP_Init();
    
    sendSUSEVR(SUS_INITDONE);
}

void run(void){    
    implementRx();
    
    processCommand();
    
#ifdef LOOPOUT
    LATAbits.LATA7 = (unsigned char)(!LATAbits.LATA7);
#endif
}

void processCommand(void){
    //Check if there's a pending command
    if(msgProcessPtr == msgRxPtr){
        return;
    }
    struct rx_message_t *cmd = &messageBuf[msgProcessPtr];
    switch(cmd->type){
        default:
            break;
    }
    msgProcessPtr++;
    if(msgProcessPtr == RX_MSG_QUEUE){msgProcessPtr = 0;}
}


//void __interrupt(irq(default), low_priority) DEFISR(void){
//    return;
//}

void halt(){
    while(TRUE){
    }
}