#ifndef COMMCONSTANTS_H
#define	COMMCONSTANTS_H

#include <xc.h> 
#include "common.h"

enum CmdType{
    CMD_NOOP =      128,
    CMD_MOTEN =     129,
    CMD_SLSET =     130,
    CMD_HOME =      131,
    CMD_RESET =     132,
    CMD_SETTLMP =   133,
    CMD_SDF =  0x06,
    CMD_VL6180DIS = 0x07,
    CMD_LEDON =     0x08,
    CMD_LEDOFF =    0x09,
    CMD_LEDBRTAUTO =0x0A,
    CMD_LEDBRTMAN = 0x0B,
    CMD_LEDBRT =    0x0C,
};

enum TlmType{
    TLM_ACK =       0,
    TLM_NAK =       1,
    TLM_SYSTSTAT =  2,
    TLM_EVR =       3,
    TLM_COMMERR =   4,
    TLM_MOTION =    5,
    TLM_HOMDIFF =   6,
    TLM_M1STAT =    7,
    TLM_M2STAT =    8,
    TLM_M3STAT =    9,
    TLM_MODE =      34,
    TLM_REG =       10,
};

enum EVRID {
    EVR_RESERVED =      0x00,
    EVR_STARTSEQ =      0x01,
    EVR_SWVER =         0x02,
};

enum StartupSeqLocation {
    SUS_INITIAL =       0x00,
    SUS_INITDONE =      0x07,
};

#endif

