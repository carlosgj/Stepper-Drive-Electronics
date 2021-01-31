#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded.  

//#define HUMAN_READABLE_SERIAL

#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define _XTAL_FREQ 48000000

#define SW_VER_STR "0.0.1"

#define HINTEN INTCON0bits.GIEH = TRUE
#define HINTDIS INTCON0bits.GIEH = FALSE

#define LINTEN INTCON0bits.GIEL = TRUE
#define LINTDIS INTCON0bits.GIEL = FALSE

unsigned int msCount = 0;

void ItoA(unsigned char val, char* dest);

#endif

