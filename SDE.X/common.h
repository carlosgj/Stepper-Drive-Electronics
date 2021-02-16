#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//######### Debug options ###########

//Uncomment to provide human-readable ASCII serial messages, instead of HDLC
//#define HUMAN_READABLE_SERIAL

//Uncomment AT MOST ONE of the following to send signals to the test point
//#define LOOPOUT
//#define MSOUT

//Uncomment to send one byte at a time to the UART, instead of using software buffering
//#define UNBUFFERED_SER

//Uncomment to send test patterns in packets instead of real telemetry
//#define TLM_TEST_PATTERN


#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define _XTAL_FREQ (32000000)

#define SW_VER_STR "0.0.1"

#define INTEN (INTCONbits.GIE = TRUE)
#define INTDIS (INTCONbits.GIE = FALSE)

enum SPIDest{
    MC = 0,
    DRV1 = 1, 
    DRV2 = 2, 
    DRV3 = 3
};

enum MotorState{
    MS_IDLE,
    MS_HOMING,
    MS_HOMED
};

#define MOTOR_STAT_LEN (7)
union MotorStatus_t {
    unsigned char all[MOTOR_STAT_LEN];
    struct{
        uint24_t actual;
        uint24_t target;
        unsigned TMC2130Stat:2;
        unsigned homeStat   :2;
        unsigned leftLimit  :1;
        unsigned rightLimit :1;
        unsigned onTarget   :1;
        unsigned RESERVED   :1;
    };
} M1Stat, M2Stat, M3Stat;

#define SYST_ERR_LEN (1)
union SystErr_t {
    unsigned char all[SYST_ERR_LEN];
    struct{
        unsigned char unhandledInt;
    };
} systErr;

#define SYST_STAT_LEN (3)
union SystStatus_t{
    unsigned char all[SYST_STAT_LEN];
    struct{
        uint16_t inputVoltage;
        unsigned motEn  :1;
        unsigned TMC429Stat :2;
        unsigned RESERVED :5;
    };
} systStat;

#define TMC429_MOTOR_SETTINGS_LEN (6)
union TMC429MotorSettings_t {
    unsigned char all[TMC429_MOTOR_SETTINGS_LEN];
    struct{
        uint16_t VMIN;
        uint16_t VMAX;
        uint16_t AMAX; 
    };
};

union U24Bytes_t {
    uint24_t all;
    struct{
        unsigned char low;
        unsigned char mid;
        unsigned char high;
    };
};

unsigned int msCount = 0;

void ItoA(unsigned char val, char* dest);

#endif

