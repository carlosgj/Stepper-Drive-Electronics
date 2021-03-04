#ifndef TMC429_H
#define	TMC429_H

#include <xc.h> 
#include <stdint.h>
#include <string.h>
#include "common.h"
#include "SPI.h"

#define MC_PREFIX_MOTOR1    (0)
#define MC_PREFIX_MOTOR2    (16)
#define MC_PREFIX_MOTOR3    (32)
#define MC_PREFIX_COMMON    (48)
    
#define MC_MOTOR_TARGET     (0)
#define MC_MOTOR_ACTUAL     (1)
#define MC_MOTOR_VMIN       (2)
#define MC_MOTOR_VMAX       (3)
#define MC_MOTOR_VTARGET    (4)
#define MC_MOTOR_VACTUAL    (5)
#define MC_MOTOR_AMAX       (6)
#define MC_MOTOR_AACTUAL    (7)
#define MC_MOTOR_ATHRESH    (8)
#define MC_MOTOR_DECCEL     (9)
#define MC_MOTOR_REFCONF    (10)
#define MC_MOTOR_INTCONF    (11)
#define MC_MOTOR_ACCEL      (12)
#define MC_MOTOR_DXREFTOL   (13)
#define MC_MOTOR_LATCHED    (14)
#define MC_MOTOR_USTEP      (15)
    
#define MC_COMMON_DGLOW     (0)
#define MC_COMMON_DGHIGH    (1)
#define MC_COMMON_COVER     (2)
#define MC_COMMON_COVERDG   (3)
#define MC_COMMON_IFCONFIG  (4)
#define MC_COMMON_POSCOMP   (5)
#define MC_COMMON_PCINT     (6)
#define MC_COMMON_RESVD1    (7)
#define MC_COMMON_PWRDN     (8) 
#define MC_COMMON_VERSION   (9)
#define MC_COMMON_RESVD2    (10)
#define MC_COMMON_RESVD3    (11)
#define MC_COMMON_RESVD4    (12)
#define MC_COMMON_RESVD5    (13)
#define MC_COMMON_RESVD6    (14)
#define MC_COMMON_CONFIG    (15)

#define TMC429_UNINIT       (0)
#define TMC429_GOOD         (1)
#define TMC429_BAD_VERSION  (2)

#define TMC429_PERIOD (10)

typedef union TMC429_Tx_Datagram_t {
    unsigned char bytes[4];
    struct{
        uint24_t data;
        unsigned read   :1;
        unsigned addr   :6;
        unsigned RRS    :1;
    };
} TMC429_Tx_Datagram;

typedef union TMC429_Rx_Datagram_t {
    unsigned char bytes[4];
    struct{
        uint24_t data;
        unsigned eqt1   :1;
        unsigned rs1    :1;
        unsigned eqt2   :1;
        unsigned rs2    :1;
        unsigned eqt3   :1;
        unsigned rs3    :1;
        unsigned cdgw   :1;
        unsigned int    :1;
    };
} TMC429_Rx_Datagram;

union MC_IFCONFIG_t {
    uint24_t all;
    struct {
        unsigned inv_ref    :1;
        unsigned sdo_int    :1;
        unsigned step_half  :1;
        unsigned inv_step   :1;
        unsigned inv_dir    :1;
        unsigned en_sd      :1;
        unsigned pos_comp_sel:1;
        unsigned en_refr    :1;
        //unsigned PADDING    :16;
    };
};

enum MotorHomingStep {
    HS_INIT,
    HS_MOVEOFF,
    HS_ISOFF,
    HS_MOVETO, 
    HS_MOVEBACK
} M1HomeStep, M2HomeStep, M3HomeStep;

uint24_t M1HomingDifference, M2HomingDifference, M3HomingDifference;
uint24_t M1TrigPos, M2TrigPos, M3TrigPos;

unsigned char TMC429_read_reg(unsigned char addr, uint24_t *data);
unsigned char TMC429_write_reg(unsigned char addr, uint24_t data);
void TMC429Init(void);
void TMC429Periodic(void);
inline void TMC429Homing(enum SPIDest motor);
inline void primeLatched(enum SPIDest motor);
inline uint24_t getLatched(enum SPIDest motor);
inline void setTargetPos(enum SPIDest motor, uint24_t pos);
inline void setActualPos(enum SPIDest motor, uint24_t pos);
#endif

