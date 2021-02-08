#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> 
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "comm.h"
#include "HDLC.h"
#include "RS422.h"
#include "time.h"
#include "SPI.h"
#include "EEPROM.h"
#include "config.h"
#include "TMC2130.h"
#include "TMC429.h"

#define SYST_ERR_LEN (4)

union SystErr_t {
    unsigned char all[SYST_ERR_LEN];
    struct{
        unsigned char unhandledInt;
    };
} systErr;

void main(void);
inline void init(void);
inline void run(void);
inline void halt(void);
void processCommand(void);

#endif	

