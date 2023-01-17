#ifndef COMMCONSTANTS_H
#define	COMMCONSTANTS_H

#include <xc.h> 
#include "common.h"

enum CmdType{
    CMD_NOOP =          128,
    CMD_MOTEN =         129,
    CMD_SSET =          130,
    CMD_LSET =          131,
    CMD_HOME =          132,
    CMD_RESET =         133,
    CMD_SETTLMP =       134,
    CMD_T429READ =      135,
    CMD_T429WRITE =     136,
    CMD_T2130READ =     137,
    CMD_T2130WRITE=     138,
    CMD_SETTARG =       139,
    CMD_SETACT =        140,
    
    CMD_SETMOTDIR =     141,
    CMD_SETHOLDCURR =   142,
    CMD_SETRUNCURR =    143,
    CMD_SETHLDRMPTIME = 144,
    CMD_SETHLDDELAY =   145,
    CMD_ENDBLEDGESTEP = 146,
    CMD_ENMSINTERP =    147,
    CMD_SETMS =         148,
};

enum TlmType{
    TLM_ACK =       0,
    TLM_NAK =       1,
    TLM_SYSTSTAT =  2,
    TLM_EVR =       3,
    TLM_SYSTERR =   4,
    TLM_COMMERR =   5,
    TLM_EEPERR =    6,
    TLM_HOMDIFF =   7,
    TLM_M1STAT =    8,
    TLM_M2STAT =    9,
    TLM_M3STAT =    10,
    TLM_T429REG =   11,
    TLM_T2130REG =  12
};

enum EVRID {
    EVR_RESERVED =      0x00,
    EVR_STARTSEQ =      0x01,
    EVR_SWVER =         0x02,
};

enum StartupSeqLocation {
    SUS_INITIAL =       0x00,
    SUS_SPIDONE =       0x01,
    SUS_EEPDONE =       0x02,
    SUS_ADCDONE =       0x03,
    SUS_TMC429DONE =    0x04,
    SUS_TMC2130DONE =   0x05,
    SUS_INITDONE =      0x07,
};

#endif

