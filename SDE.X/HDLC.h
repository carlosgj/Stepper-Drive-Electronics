#ifndef HDLC_H
#define	HDLC_H

#include <xc.h>
#include <string.h>
#include "common.h"
#include "RS422.h"
#include "commConstants.h"

#define HDLC_BOUNDARY   (0x7e)
#define HDLC_ESCAPE     (0x7d)
#define TIMESYNC_ARM    (0x7c)
#define MAX_RX_SIZE     (16)
#define RX_MSG_QUEUE    (4)
#define FRAMEBUF_SIZE   (MAX_RX_SIZE+2)

struct rx_message_t{
    enum CmdType type;
    unsigned char payload[MAX_RX_SIZE];
    unsigned char payloadLen;
};

struct rx_message_t messageBuf[RX_MSG_QUEUE];
unsigned char msgRxPtr = 0;
unsigned char msgProcessPtr = 0;

unsigned char frameBuf[FRAMEBUF_SIZE];
unsigned char framePtr = 0;
unsigned char thisFrameSize = 0;

#define MSGBUF_WAITING ((unsigned char)(msgRxPtr-msgProcessPtr))

void HDLCInit(void);

void sendBuf(unsigned char *buf, unsigned int count, enum TlmType commandByte);

//This function is responsible for taking bytes out of the 422 Rx queue and 
// chunking them up into HDLC frames.
void implementRx(void);

//This function checks the contents of frameBuf to make sure it's a valid HDLC frame, 
// and if so, extracts it into a message
void validateFrame(unsigned char len);
#endif

