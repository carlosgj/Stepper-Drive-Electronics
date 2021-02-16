#include <xc.h>
#include "main.h"

void main(void) {
    init();
    while(TRUE){
        CLRWDT();
        run();
    }
}

inline void init(void){
    INTDIS;
    //Setup pins
    TRISAbits.TRISA0 = OUTPUT;
    disableMotors();
    
    //Setup oscillator
    OSCCONbits.SPLLEN = TRUE; //Enable 4x PLL
    OSCCONbits.IRCF = 0b1110; //8 MHz (32 MHz with PLL)
    
#ifdef LOOPOUT
    TRISAbits.TRISA7 = OUTPUT;
#endif
    INTEN;
    
    //All pins to digital
    ANSELA = 0;
    ANSELB = 0;
    
    memset(systErr.all, 0, SYST_ERR_LEN); //Initialize system error counters
    systStat.TMC429Stat = TMC429_UNINIT;
    systStat.inputVoltage = 0x0000;
    
    timerInit();
    __delay_ms(400);
    
    RS422_Init();
    HDLCInit();
    commInit();
    sendSUSEVR(SUS_INITIAL);
    sendSwVerEVR();
    
    //Initialize SPI
    SPIInit();
    sendSUSEVR(SUS_SPIDONE);
    
    //Initialize EEPROM
    EEP_Init();
    sendSUSEVR(SUS_EEPDONE);
    
    //Initialize ADC
    ADC_Init();
    sendSUSEVR(SUS_ADCDONE);
    
    //Initialize TMC429
    TMC429Init();
    sendSUSEVR(SUS_TMC429DONE);
    
    //Initialize TMC2130s
    TMC2130Init();
    sendSUSEVR(SUS_TMC2130DONE);
    
    sendSUSEVR(SUS_INITDONE);
    
    INTCONbits.PEIE = TRUE; //Enable peripheral interrupts
}

inline void run(void){    
    implementRx();
    processCommand();
    TMC429Periodic();
    //TMC2130Periodic();
    getInputVoltage();
    sendTlm();
    __delay_ms(10);
    CLRWDT();
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
        case CMD_NOOP:
            //Send ack
            sendBuf((unsigned char *)0, 0, TLM_ACK);
            break;
        default:
            commErrors.unkOpcode++;
            break;
    }
    msgProcessPtr++;
    if(msgProcessPtr == RX_MSG_QUEUE){msgProcessPtr = 0;}
}

void getInputVoltage(void){
    takeMeasurement(ADCH_AN2, 4, &(systStat.inputVoltage));
}

inline void enableMotors(){
    LATAbits.LATA0 = FALSE;
    systStat.motEn = TRUE;
}

inline void disableMotors(){
    LATAbits.LATA0 = TRUE;
    systStat.motEn = FALSE;
}

void interrupt ISR(void){
    
    if(PIE1bits.RCIE && PIR1bits.RCIF){
        RS422RXISR();
        return;
    }
    
    if(PIE1bits.TXIE && PIR1bits.TXIF){
        RS422TXISR();
        return;
    }
    
    if(INTCONbits.TMR0IF){
        TMR0ISR();
        return;
    }
    //Unhandled interrupt
    systErr.unhandledInt++;
}

inline void halt(){
    while(TRUE){
    }
}