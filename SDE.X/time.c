#include <xc.h>
#include "time.h"

void timerInit(void){
    OPTION_REGbits.T0CS = FALSE; //Source is Fosc/4
    OPTION_REGbits.PSA = FALSE; //Prescaler assigned
    OPTION_REGbits.PS = 0b100; //1:32 prescaler
    TMR0 = MS_TMR_VAL;
    
#ifdef MSOUT
    TRISAbits.TRISA7 = OUTPUT;
    LATAbits.LATA7 = FALSE;
#endif
    
    INTCONbits.TMR0IE = TRUE;
}

//Implement ms timer
inline void TMR0ISR(void){
    msCount++;
#ifdef DEBUG_TIMER
    LATAbits.LATA7 = !LATAbits.LATA7;
#endif
    INTCONbits.TMR0IF = FALSE;
    TMR0 = MS_TMR_VAL;
}