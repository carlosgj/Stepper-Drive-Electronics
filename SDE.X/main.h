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
#include "adc.h"
//#include "motioncontrol.h"

void main(void);
inline void init(void);
inline void run(void);
void getInputVoltage(void);
inline void halt(void);
void processCommand(void);
inline void enableMotors(void);
inline void disableMotors(void);

#endif	

