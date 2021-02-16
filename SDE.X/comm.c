#include <xc.h>
#include "comm.h"

void commInit(void){
    memset(commErrors.all, 0, COMM_ERR_LEN);
    
    memcpy(TlmPeriodTab.all, defaultTlmPeriods.all, TLM_PERIOD_TAB_LEN);
    
    //Setup tlm times
    //memcpy(TlmElapsed.all, TlmPeriodTab.all, TLM_PERIOD_TAB_LEN);
    TlmElapsed.CommErr = 0;
    TlmElapsed.EEPErr = 20;
    TlmElapsed.SystErr = 40;
    TlmElapsed.SystStat = 80;
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
        sendMotorStatTlm(DRV1);
        TlmElapsed.M1Stat = TlmPeriodTab.M1Stat;
    }
    else{
        TlmElapsed.M1Stat--;
    }
    
    if(TlmElapsed.M2Stat == 0){
        sendMotorStatTlm(DRV2);
        TlmElapsed.M2Stat = TlmPeriodTab.M2Stat;
    }
    else{
        TlmElapsed.M2Stat--;
    }
    
    if(TlmElapsed.M3Stat == 0){
        sendMotorStatTlm(DRV3);
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
#ifdef TLM_TEST_PATTERN
    commErrors.txBuffOvf = 1;
    commErrors.rxBuffOvf = 2;
    commErrors.frameBufOvf = 3;
    commErrors.msgBufOvf = 4;
    commErrors.byteStuff = 5;
    commErrors.oversizeFrame = 6;
    commErrors.framing = 7;
    commErrors.unkOpcode = 8;
#endif
    sendBuf(commErrors.all, COMM_ERR_LEN, TLM_COMMERR);
}

void sendSystStatTlm(void){
#ifdef TLM_TEST_PATTERN
    systStat.inputVoltage = 0x0201;
    systStat.TMC429Stat = 1;
    systStat.motEn = 1;
#endif
    sendBuf(systStat.all, SYST_STAT_LEN, TLM_SYSTSTAT);
}

void sendSystErrorTlm(void){
#ifdef TLM_TEST_PATTERN
    systErr.unhandledInt = 0x0f;
#endif
    sendBuf(systErr.all, SYST_ERR_LEN, TLM_SYSTERR);
}

void sendEEPROMErrorTlm(void){
#ifdef TLM_TEST_PATTERN
    EEPErrors.complement = 3;
    EEPErrors.readback = 2;
    EEPErrors.writeTimeout = 1;
#endif
    sendBuf(EEPErrors.all, EEP_ERR_LEN, TLM_EEPERR);
}

void sendMotorStatTlm(enum SPIDest motor){
    switch(motor){
        case DRV1:
#ifdef TLM_TEST_PATTERN
            M1Stat.target = 0x060504;
            M1Stat.actual = 0x030201;
            M1Stat.TMC2130Stat = 1;
            M1Stat.homeStat = 3;
            M1Stat.leftLimit = 0;
            M1Stat.rightLimit = 1;
#endif
            sendBuf(M1Stat.all, MOTOR_STAT_LEN, TLM_M1STAT);
            break;
        case DRV2:
#ifdef TLM_TEST_PATTERN
            M2Stat.target = 0x060504;
            M2Stat.actual = 0x030201;
            M2Stat.TMC2130Stat = 1;
            M2Stat.homeStat = 3;
            M2Stat.leftLimit = 0;
            M2Stat.rightLimit = 1;
#endif
            sendBuf(M2Stat.all, MOTOR_STAT_LEN, TLM_M2STAT);
            break;
        case DRV3:
#ifdef TLM_TEST_PATTERN
            M3Stat.target = 0x060504;
            M3Stat.actual = 0x030201;
            M3Stat.TMC2130Stat = 1;
            M3Stat.homeStat = 3;
            M3Stat.leftLimit = 0;
            M3Stat.rightLimit = 1;
#endif
            sendBuf(M3Stat.all, MOTOR_STAT_LEN, TLM_M3STAT);
            break;
        default:
            return;
    }
}