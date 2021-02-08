#ifndef SPI_H
#define	SPI_H

#include <xc.h> 
#include "common.h"

#define SPI_TIMEOUT (255); //TODO

#define MCLAT (LATAbits.LATA1)
#define DRV1LAT (LATAbits.LATA3)
#define DRV2LAT (LATBbits.LATB0)
#define DRV3LAT (LATAbits.LATA4)

#define MCTRIS (TRISAbits.TRISA1)
#define DRV1TRIS (TRISAbits.TRISA3)
#define DRV2TRIS (TRISBbits.TRISB0)
#define DRV3TRIS (TRISAbits.TRISA4)

#define MISOTRIS (TRISBbits.TRISB1)
#define MOSITRIS (TRISAbits.TRISA6)

void SPIInit(void);

unsigned char SPIXfer(enum SPIDest dest, unsigned char *txBuf, unsigned char *rxBuf, unsigned char count);

#endif

