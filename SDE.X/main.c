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
    __delay_ms(10);
    //Initialize SPI
    SPIInit();
    sendSUSEVR(SUS_SPIDONE);
    __delay_ms(10);
    //Initialize EEPROM
    EEP_Init();
    sendSUSEVR(SUS_EEPDONE);
    __delay_ms(10);
    //Initialize ADC
    ADC_Init();
    sendSUSEVR(SUS_ADCDONE);
    __delay_ms(10);
    //Initialize TMC429
    TMC429Init();
    sendSUSEVR(SUS_TMC429DONE);
    __delay_ms(10);
    //Initialize TMC2130s
    TMC2130InitAll();
    sendSUSEVR(SUS_TMC2130DONE);
    __delay_ms(10);
    sendSUSEVR(SUS_INITDONE);
    
    INTCONbits.PEIE = TRUE; //Enable peripheral interrupts
}

inline void run(void){    
    implementRx();
    processCommand();
    TMC429Periodic();
    TMC2130Periodic();
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
    union U24Bytes_t val24;
    union U32Bytes_t val32;
    struct rx_message_t *cmd = &messageBuf[msgProcessPtr];
    switch(cmd->type){
        case CMD_NOOP:
            //Send ack
            sendBufBE((unsigned char *)0, 0, TLM_ACK);
            break;
        case CMD_MOTEN:
            if(cmd->payloadLen != 1){
                //Malformed command
                break;
            }
            if(cmd->payload[0] == 1){
                enableMotors();
            }
            else{
                disableMotors();
            }
            break;
        case CMD_T429READ:
            TMC429_read_reg(cmd->payload[0], &(val24.all));
            sendBufBE(val24.bytes, 3, TLM_T429REG);
            break;
        case CMD_T429WRITE:
            val24.high = cmd->payload[1];
            val24.mid = cmd->payload[2];
            val24.low = cmd->payload[3];
            TMC429_write_reg(cmd->payload[0], val24.all);
            break;
        case CMD_T2130READ:
            TMC2130_read_reg((enum SPIDest)(cmd->payload[0]), cmd->payload[1], &(val32.all));
            sendBufBE(val32.bytes, 4, TLM_T2130REG);
            break;
        case CMD_T2130WRITE:
            val32.high = cmd->payload[2];
            val32.hmid = cmd->payload[3];
            val32.lmid = cmd->payload[4];
            val32.low = cmd->payload[5];
            TMC2130_write_reg(cmd->payload[0], cmd->payload[1], val32.all);
            break;
        case CMD_SETTARG:
            val24.high = cmd->payload[1];
            val24.mid = cmd->payload[2];
            val24.low = cmd->payload[3];
            setTargetPos((enum SPIDest)(cmd->payload[0]), val24.all);
            break;
        case CMD_HOME:
            if(cmd->payloadLen != 1){
                //Malformed command
                //TODO: Error handling
                break;
            }
            switch(cmd->payload[0]){
                case 1:
                    //Home motor 1
                    if(M1Stat.homeStat == MS_IDLE || M1Stat.homeStat == MS_HOMED){
                        M1Stat.homeStat = MS_HOMING;
                        M1HomeStep = HS_INIT;
                    }
                    else{
                        //TODO: Error handling
                    }
                    break;
                case 2:
                    //Home motor 2
                    if(M2Stat.homeStat == MS_IDLE || M2Stat.homeStat == MS_HOMED){
                        M2Stat.homeStat = MS_HOMING;
                        M2HomeStep = HS_INIT;
                    }
                    else{
                        //TODO: Error handling
                    }
                    break;
                case 3:
                    //Home motor 3
                    if(M3Stat.homeStat == MS_IDLE || M3Stat.homeStat == MS_HOMED){
                        M3Stat.homeStat = MS_HOMING;
                        M3HomeStep = HS_INIT;
                    }
                    else{
                        //TODO: Error handling
                    }
                    break;
                default:
                    //TODO: error handling
                    break;
            }
            break;
            
        case CMD_SETMOTDIR:
            setMotorDirection(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_SETHOLDCURR:
            setHoldCurrent(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_SETRUNCURR:
            setRunCurrent(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_SETHLDRMPTIME:
            setHoldCurrentRampTime(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_SETHLDDELAY:
            setHoldDelay(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_ENDBLEDGESTEP:
            setDoubleEdgeStep(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_ENMSINTERP:
            setMicrostepInterpolation(cmd->payload[0], cmd->payload[1]);
            break;
        case CMD_SETMS:
            setMicrostepping(cmd->payload[0], cmd->payload[1]);
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
    M1Stat.homeStat = MS_IDLE;
    M2Stat.homeStat = MS_IDLE;
    M3Stat.homeStat = MS_IDLE;
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