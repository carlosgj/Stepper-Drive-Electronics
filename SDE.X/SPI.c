#include "SPI.h"

void SPIInit(void){
    SSP1ADD = 7; //7 should result in 1 MHz with 32 MHz Fosc
    
    //Set up SPI pin locations
    APFCON0bits.SDO1SEL = TRUE; //SDO1 on RA6
    
    //Set SPI directions
    CLKTRIS = OUTPUT;
    MISOTRIS = INPUT;
    MOSITRIS = OUTPUT;
    
    //Setup MSSP
    SSP1STATbits.SMP = FALSE; //TODO
    SSP1STATbits.CKE = FALSE; //TODO
    SSP1CON1bits.CKP = FALSE; //TODO
    SSP1CON1bits.SSPM = 0b0000; //SPI master, clock = Fosc/4
    
    //Set CS directions
    MCTRIS = OUTPUT;
    DRV1TRIS = OUTPUT;
    DRV2TRIS = OUTPUT;
    DRV3TRIS = OUTPUT;
    
    //Write CS pins high
    MCLAT = TRUE;
    DRV1LAT = TRUE;
    DRV2LAT = TRUE;
    DRV3LAT = TRUE;
    
    //Turn on SSP
    SSP1CON1bits.SSPEN = TRUE;
}

unsigned char SPIXfer(enum SPIDest dest, unsigned char *txBuf, unsigned char *rxBuf, unsigned char count){
    switch(dest){
        case MC:
            MCLAT = FALSE;
            break;
        case DRV1:
            DRV1LAT = FALSE;
            break;
        case DRV2:
            DRV2LAT = FALSE;
            break;
        case DRV3:
            DRV3LAT = FALSE;
            break;
        default:
            return 1; //TODO
            break;
    }
    
    unsigned char i;
    unsigned char timeout;
    for(i=0; i<count; i++){
        SSP1IF = FALSE;
        timeout = SPI_TIMEOUT;
        SSP1BUF = txBuf[i];
        while(!SSP1IF){
            timeout--;
            if(timeout==0){
                break;
                //TODO: error handling
            }
        }
        rxBuf[i] = SSP1BUF;
    }
    
    switch(dest){
        case MC:
            MCLAT = TRUE;
            break;
        case DRV1:
            DRV1LAT = TRUE;
            break;
        case DRV2:
            DRV2LAT = TRUE;
            break;
        case DRV3:
            DRV3LAT = TRUE;
            break;
        default:
            return 1; //TODO
            break;
    }
    return 0; //TODO
}