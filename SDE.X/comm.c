#include <xc.h>
#include "comm.h"

void commInit(void){
    memset(commErrors.all, 0, COMM_ERR_LEN);
    
    TlmPeriodTab.CommErr = 100;
    TlmPeriodTab.SystErr = 100;
    
    //Setup tlm times
    memcpy(TlmElapsed.all, TlmPeriodTab.all, TLM_PERIOD_TAB_LEN);
}

void sendTlm(){
    if(TlmElapsed.SystStat == 0){
        sendSystStatTlm();
        TlmElapsed.SystStat = TlmPeriodTab.SystStat;
    }
    else{
        TlmElapsed.SystStat--;
    }
    
    if(TlmElapsed.SystErr == 0){
        sendSystErrorTlm();
        TlmElapsed.SystErr = TlmPeriodTab.SystErr;
    }
    else{
        TlmElapsed.SystErr--;
    }
    
    if(TlmElapsed.CommErr == 0){
        sendCommErrorTlm();
        TlmElapsed.CommErr = TlmPeriodTab.CommErr;
    }
    else{
        TlmElapsed.CommErr--;
    }
    
    if(TlmElapsed.EEPErr == 0){
        sendEEPROMErrorTlm();
        TlmElapsed.EEPErr = TlmPeriodTab.EEPErr;
    }
    else{
        TlmElapsed.EEPErr--;
    }
    
    if(TlmElapsed.M1Stat == 0){
        sendSystStatTlm();
        TlmElapsed.M1Stat = TlmPeriodTab.M1Stat;
    }
    else{
        TlmElapsed.M1Stat--;
    }
    
    if(TlmElapsed.M2Stat == 0){
        sendSystStatTlm();
        TlmElapsed.M2Stat = TlmPeriodTab.M2Stat;
    }
    else{
        TlmElapsed.M2Stat--;
    }
    
    if(TlmElapsed.M3Stat == 0){
        sendSystStatTlm();
        TlmElapsed.M3Stat = TlmPeriodTab.M3Stat;
    }
    else{
        TlmElapsed.M3Stat--;
    }
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

void sendSystStatTlm(void){
    
}

void sendSystErrorTlm(void){
    sendBuf(systErr.all, SYST_ERR_LEN, TLM_SYSTERR);
}

void sendEEPROMErrorTlm(void){
    
}