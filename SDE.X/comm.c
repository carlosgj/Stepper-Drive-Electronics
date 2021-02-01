#include <xc.h>
#include "comm.h"

void commInit(void){
    memset(commErrors.all, 0, COMM_ERR_LEN);
}

void sendSUSEVR(enum StartupSeqLocation loc){
    unsigned char buf[2];
    buf[0] = EVR_STARTSEQ;
    buf[1] = loc;
    sendBuf(buf, 2, TLM_EVR);
}

void sendSwVerEVR(void){
    unsigned char buf[sizeof(SW_VER_STR)+1];
    const unsigned char *ver = SW_VER_STR;
    buf[0] = EVR_SWVER;
    memcpy(buf+1, ver, sizeof(SW_VER_STR));
    sendBuf(buf, sizeof(SW_VER_STR)+1, TLM_EVR);
}

void sendCommErrorTlm(void){
    sendBuf(commErrors.all, COMM_ERR_LEN, TLM_COMMERR);
}