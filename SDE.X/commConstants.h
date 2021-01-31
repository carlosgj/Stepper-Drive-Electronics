#ifndef COMMCONSTANTS_H
#define	COMMCONSTANTS_H

#include <xc.h> 
#include "common.h"

enum CmdType{
    CMD_RESERVED0 = 0x00,
    CMD_SETTIME =   0x01,
    CMD_ADNSEN =    0x02,
    CMD_ADNSDIS =   0x03,
    CMD_FRAMEEN =   0x04,
    CMD_FRAMEDIS =  0x05,
    CMD_VL6180EN =  0x06,
    CMD_VL6180DIS = 0x07,
    CMD_LEDON =     0x08,
    CMD_LEDOFF =    0x09,
    CMD_LEDBRTAUTO =0x0A,
    CMD_LEDBRTMAN = 0x0B,
    CMD_LEDBRT =    0x0C,
};

enum TlmType{
    TLM_HEARTBEAT = 0x00,
    TLM_EVR =       0x01,
    TLM_TEXT =      0x02,
    TLM_RANGE =     0x03,
    TLM_IMQUAL =    0x04,
    TLM_MOTION =    0x05,
    TLM_IMAGE =     0x06,
    TLM_ADNSREGVAL =0x08,
    TLM_VLREGVAL =  0x09,
    TLM_COMMERROR = 0x0A,
    TLM_MODE =      0x0B,
    TLM_LEDBRT =    0x0C,
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

