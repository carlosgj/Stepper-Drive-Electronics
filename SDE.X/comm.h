#ifndef COMM_H
#define	COMM_H

#include <xc.h> 
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "HDLC.h"

#define PRINT(x) sendBuf((unsigned char *)x, sizeof(x), TLM_TEXT)
//#define PRINT(x) RS422_SendBytes((unsigned char *)x, sizeof(x))

union commError_t {
    unsigned long all;
    struct {
        unsigned char txBuffOvf;
        unsigned char rxBuffOvf;
        unsigned char frameBufOvf;
        unsigned char msgBufOvf;
    };
} commErrors;

void sendModeTlm(void);
void sendCommErrorTlm(void);
void sendRangeTlm(void);
void sendSQUALTlm(void);
void sendMotionTlm(void);
void sendMaxPxTlm(void);
void sendLEDBrightnessTlm(void);

void sendSUSEVR(enum StartupSeqLocation loc);
void sendSwVerEVR(void);

#endif

