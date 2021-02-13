#ifndef COMM_H
#define	COMM_H

#include <xc.h> 
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "HDLC.h"

#define PRINT(x) sendBuf((unsigned char *)x, sizeof(x), TLM_TEXT)
//#define PRINT(x) RS422_SendBytes((unsigned char *)x, sizeof(x))

#define COMM_ERR_LEN (8)
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
        unsigned char unkOpcode;
    };
} commErrors;

union TlmPeriodTab_t {
    unsigned char all[TLM_PERIOD_TAB_LEN];
    struct{
        unsigned char SystStat;
        unsigned char SystErr;
        unsigned char CommErr;
        unsigned char EEPErr;
        unsigned char M1Stat;
        unsigned char M2Stat;
        unsigned char M3Stat;
    };
} TlmPeriodTab, TlmElapsed;

void commInit(void);
void sendTlm(void);
void sendCommErrorTlm(void);
void sendSystStatTlm(void);
void sendSystErrorTlm(void);
void sendEEPROMErrorTlm(void);
void sendSUSEVR(enum StartupSeqLocation loc);
void sendSwVerEVR(void);

#endif

