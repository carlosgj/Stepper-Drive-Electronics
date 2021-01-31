#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded.  

//Debug options
//#define HUMAN_READABLE_SERIAL
#define LOOPOUT

#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define _XTAL_FREQ 48000000

#define SW_VER_STR "0.0.1"

#define INTEN //INTCON0bits.GIEH = TRUE
#define INTDIS //INTCON0bits.GIEH = FALSE

unsigned int msCount = 0;

void ItoA(unsigned char val, char* dest);

#endif

