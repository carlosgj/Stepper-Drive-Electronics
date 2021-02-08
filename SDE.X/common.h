#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded.  

//######### Debug options ###########

//Uncomment to provide human-readable ASCII serial messages, instead of HDLC
//#define HUMAN_READABLE_SERIAL

//Uncomment AT MOST ONE of the following to send signals to the test point
//#define LOOPOUT
//#define MSOUT

//Uncomment to send one byte at a time to the UART, instead of using software buffering
#define UNBUFFERED_SER

#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define _XTAL_FREQ (32000000)

#define SW_VER_STR "0.0.1"

#define INTEN (INTCONbits.GIE = TRUE)
#define INTDIS (INTCONbits.GIE = FALSE)

enum SPIDest{
    MC = 0,
    DRV1 = 1, 
    DRV2 = 2, 
    DRV3 = 3
};

unsigned int msCount = 0;

void ItoA(unsigned char val, char* dest);

#endif

