#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> 
#include "common.h"
#include "commConstants.h"
#include "comm.h"
#include "HDLC.h"
#include "RS422.h"
#include "time.h"

void main(void);
void init(void);
void run(void);
void halt(void);
void processCommand(void);

#endif	

