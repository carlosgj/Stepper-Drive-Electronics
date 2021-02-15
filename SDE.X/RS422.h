#ifndef RS422_H
#define	RS422_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "common.h"
#include "commConstants.h"
#include "comm.h"

#define RS422_BRG_VAL   (51)
#define TX_BUF_SIZE     (64) //TODO
#define RX_BUF_SIZE     (48) //TODO

#define TXBUF_FREE ((unsigned char)((txbufread-txbufwrite)-1))
#define RXBUF_FREE ((unsigned char)((rxbufread-rxbufwrite)-1))

//#define TXBUFWRITE(x) txbuf[txbufwrite++] = x

unsigned char txbuf[TX_BUF_SIZE];
volatile unsigned char txbufwrite = 0;
volatile unsigned char txbufread = 0;

unsigned char rxbuf[RX_BUF_SIZE];
volatile unsigned char rxbufwrite = 0;
volatile unsigned char rxbufread = 0;

void RS422_Init(void);
void RS422_TxByte(unsigned char theByte);
//void RS422_SendBytes(unsigned char *buf, unsigned char count);

//Starts transmitting txbuf
//Safe to call even if we're already transmitting
inline void RS422_StartTx(void);

inline void RS422TXISR(void);
inline void RS422RXISR(void);

#endif

