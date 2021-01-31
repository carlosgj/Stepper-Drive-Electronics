#include <xc.h>
#include "time.h"

void timerInit(void){
    //T0CON0bits.MD16 = FALSE; //8-bit mode
    //T0CON1bits.CS = 0b010; //Fosc/4
    //T0CON1bits.CKPS = 0b0110; //1:64 prescaler
    //TMR0H = 188; //Should result in 1.003 ms tick with 48MHz Fosc
    
#ifdef DEBUG_TIMER
    TRISAbits.TRISA7 = OUTPUT;
    LATAbits.LATA7 = FALSE;
#endif
    
    //PIE3bits.TMR0IE = TRUE;
    //T0CON0bits.EN = TRUE;
}

//Implement ms timer
//void __interrupt(irq(TMR0), low_priority) TMR0ISR(void){
//    msCount++;
//#ifdef DEBUG_TIMER
//    LATAbits.LATA7 = !LATAbits.LATA7;
//#endif
//    PIR3bits.TMR0IF = FALSE;
//}