#include <xc.h>
#include "adc.h"

void ADC_Init(void){
    //Set up pins
    ANSELAbits.ANSA2 = TRUE; //analog in on RA2
    TRISAbits.TRISA2 = INPUT;
    
    ADCON1bits.ADFM = TRUE; //Right-justified
    ADCON1bits.ADCS = 0b110; //Fosc/64
    ADCON1bits.ADPREF = 0b11; //FVR
    
    //Set up FVR
    FVRCONbits.ADFVR = 0b11; //4.096 V
    FVRCONbits.FVREN = TRUE; //Enable FVR
    
    //Turn on ADC
    ADCON0bits.ADON = TRUE;
}

unsigned char takeMeasurement(unsigned char channel, unsigned char numReadings, uint16_t *result){
    unsigned char success = ADC_STAT_SUCCESS;
    unsigned char i;
    unsigned char timeout;
    *result = 0;
    ADCON0bits.CHS = channel;
    for(i=0; i<numReadings; i++){
        union ADC_Res_t thisReading;
        //Wait for acq time
        __delay_us(5);
        ADCON0bits.ADGO = TRUE; //Start conversion
        timeout = ADC_CONV_TIMEOUT;
        while(ADCON0bits.ADGO){
            timeout--;
            if(timeout == 0){
                success = ADC_STAT_TIMEOUT;
                break;
            }
        }
        thisReading.high = ADRESH;
        thisReading.low = ADRESL;
        *result += thisReading.all;
    }
    return success;
}