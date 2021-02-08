#ifndef COMM_H
#define	COMM_H

#include <xc.h> 
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "HDLC.h"

#define PRINT(x) sendBuf((unsigned char *)x, sizeof(x), TLM_TEXT)
//#define PRINT(x) RS422_SendBytes((unsigned char *)x, sizeof(x))

#define COMM_ERR_LEN (7)
#define TLM_PERIOD_TAB_LEN (8)

union commError_t {
    unsigned char all[COMM_ERR_LEN];
    struct {
        unsigned char txBuffOvf;
        unsigned char rxBuffOvf;
        unsigned char frameBufOvf;
        unsigned char msgBufOvf;
        unsigned char bitStuff;
        unsigned char oversizeFrame;
        unsigned char framing;
    };
} commErrors;

union TlmPeriodTab_t {
    unsigned char all[TLM_PERIOD_TAB_LEN];
    struct{
        unsigned char SystStatPer;
        unsigned char SystErrPer;
        unsigned char CommErrPer;
        unsigned char EEPErrPer;
        unsigned char M1StatPer;
        unsigned char M2StatPer;
        unsigned char M3StatPer;
    };
} TlmPeriodTab;

void commInit(void);
void sendCommErrorTlm(void);

void sendSUSEVR(enum StartupSeqLocation loc);
void sendSwVerEVR(void);

#endif

