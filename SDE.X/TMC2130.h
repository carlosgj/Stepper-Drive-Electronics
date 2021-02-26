#ifndef TMC2130_H
#define	TMC2130_H

#include <xc.h> 
#include <stdint.h>
#include "common.h"
#include "SPI.h"

#define DRV_REG_GCONF       (0x00)
#define DRV_REG_GSTAT       (0x01)
#define DRV_REG_IOIN        (0x04)
#define DRV_REG_IHOLD_IRUN  (0x10)
#define DRV_REG_TPOWERDOWN  (0x11)
#define DRV_REG_TSTEP       (0x12)
#define DRV_REG_TPWMTHRS    (0x13)
#define DRV_REG_TCOOLTHRS   (0x14)
#define DRV_REG_THIGH       (0x15)
#define DRV_REG_XDIRECT     (0x2d)
#define DRV_REG_VDCMIN      (0x33)
#define DRV_REG_MSLUT0      (0x60)
#define DRV_REG_MSLUT1      (0x61)
#define DRV_REG_MSLUT2      (0x62)
#define DRV_REG_MSLUT3      (0x63)
#define DRV_REG_MSLUT4      (0x64)
#define DRV_REG_MSLUT5      (0x65)
#define DRV_REG_MSLUT6      (0x66)
#define DRV_REG_MSLUT7      (0x67)
#define DRV_REG_MSLUTSEL    (0x68)
#define DRV_REG_MSLUTSTART  (0x69)
#define DRV_REG_MSCNT       (0x6a)
#define DRV_REG_MSCURACT    (0x6b)
#define DRV_REG_CHOPCONF    (0x6c)
#define DRV_REG_COOLCONF    (0x6d)
#define DRV_REG_DCCTRL      (0x6e)
#define DRV_REG_DRV_STATUS  (0x6f)
#define DRV_REG_PWMCONF     (0x70)
#define DRV_REG_PWM_SCALE   (0x71)
#define DRV_REG_ENCM_CTRL   (0x72)
#define DRV_REG_LOST_STEPS  (0x73)

#define TMC2130_UNINIT       (0)
#define TMC2130_GOOD         (1)
#define TMC2130_BAD_VERSION  (2)

typedef union TMC2130_Tx_Datagram_t {
    unsigned char bytes[5];
    struct{
        uint32_t data;
        unsigned addr   :7;    
        unsigned write  :1;
    };
} TMC2130_Tx_Datagram;

typedef union TMC2130_Rx_Datagram_t {
    unsigned char bytes[5];
    struct{
        uint32_t  data;
        unsigned reset  :1;
        unsigned drv_err:1;
        unsigned sg2    :1;
        unsigned ststill:1;
        unsigned unused0:1;
        unsigned unused1:1;
        unsigned unused2:1;
        unsigned unused3:1;
    };
} TMC2130_Rx_Datagram;

union TMC2130_GCONF_t {
    uint32_t all;
    unsigned char bytes[4];
    struct{
        unsigned I_scale_analog     :1;
        unsigned internal_Rsense    :1;
        unsigned en_pwm_mode        :1;
        unsigned enc_communication  :1;
        unsigned shaft              :1;
        unsigned diag0_error        :1;
        unsigned diag0_otpw         :1;
        unsigned diag0_stall        :1;
        unsigned diag1_stall        :1;
        unsigned diag1_index        :1;
        unsigned diag1_onstate      :1;
        unsigned diag1_steps_skipped:1;
        unsigned diag0_int_pushpull :1;
        unsigned diag1_pushpull     :1;
        unsigned small_hysteresis   :1;
        unsigned stop_enable        :1;
        unsigned direct_mode        :1;
        unsigned test_mode          :1;
    };
};

union TMC2130_GSTAT_t {
    uint32_t all;
    unsigned char bytes[4];
    struct {
        unsigned reset      :1;
        unsigned drv_err    :1;
        unsigned uv_cp      :1;
    };
};

void TMC2130Init(void);
unsigned char TMC2130_read_reg(enum SPIDest target, unsigned char addr, uint32_t *data);
unsigned char TMC2130_write_reg(enum SPIDest target, unsigned char addr, uint32_t data);
void TMC2130Periodic(void);
#endif

