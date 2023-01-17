#include <xc.h>
#include "TMC2130.h"

void TMC2130InitAll(void){
    TMC2130InitShadowRegs();
    TMC2130Init(DRV1);
    TMC2130Init(DRV2);
    TMC2130Init(DRV3);
}

void TMC2130InitShadowRegs(void){
    TMC2130ShadowRegs.IHOLD_IRUN.all = DRV_REG_IHOLD_IRUN_DEFAULT;
    TMC2130ShadowRegs.TCOOLTHRS = 0;
    TMC2130ShadowRegs.THIGH = 0;
    TMC2130ShadowRegs.TPOWERDOWN = 0;
    TMC2130ShadowRegs.TPWMTHRS = 0;
}

void TMC2130Init(enum SPIDest drv){
    uint32_t data;
    union MotorStatus_t *stat;
    
    switch(drv){
        case DRV1:
            stat = &M1Stat;
            break;
        case DRV2:
            stat = &M2Stat;
            break;
        case DRV3:
            stat = &M3Stat;
            break;
        default:
            //TODO: Error handling
            return;
    }
    
    TMC2130_read_reg(drv, DRV_REG_GSTAT, &data);
    
    //Verify ID reg
    TMC2130_read_reg(drv, DRV_REG_IOIN, &data);
    if((data >> 24) == 0x11){
        stat->TMC2130Stat = TMC2130_GOOD;
    }
    else{
        stat->TMC2130Stat = TMC2130_BAD_VERSION;
    }
}

void TMC2130Periodic(void){
    static unsigned char counter = 10;
    if(counter-- != 0){
        return;
    }
    unsigned char i;
    for(i=1; i<4; i++){
        enum SPIDest target = (enum SPIDest)(i);
        
        //Read GSTAT
        union TMC2130_GSTAT_t gstat;
        TMC2130_read_reg(target, DRV_REG_GSTAT, &gstat.all);
        //TODO: handle gstat stuff
    }
    
    
    counter = 10;
}

unsigned char TMC2130_read_reg(enum SPIDest target, unsigned char addr, uint32_t *data){
    unsigned char success;
    TMC2130_Tx_Datagram datagram;
    TMC2130_Rx_Datagram result;
    datagram.write = FALSE;
    datagram.addr = addr;
    datagram.data = 0;
    
    //Setup address
    SPIXfer(target, datagram.bytes, result.bytes, 5);
    //Actually get data
    success = SPIXfer(target, datagram.bytes, result.bytes, 5);
    
    if(success != 0){
        return success;
    }
    
    //TODO: handle status bits in result
    
    *data = result.data;
    return 0;
}

unsigned char TMC2130_write_reg(enum SPIDest target, unsigned char addr, uint32_t data){
    unsigned char success;
    TMC2130_Tx_Datagram datagram;
    TMC2130_Rx_Datagram result;
    datagram.write = TRUE;
    datagram.addr = addr;
    datagram.data = data;
    
    success = SPIXfer(target, datagram.bytes, result.bytes, 5);
    
    if(success != 0){
        switch(addr){
            case DRV_REG_IHOLD_IRUN:
                TMC2130ShadowRegs.IHOLD_IRUN.all = data;
                break;
            case DRV_REG_TPOWERDOWN:
                TMC2130ShadowRegs.TPOWERDOWN = data;
                break;
            case DRV_REG_TPWMTHRS:
                TMC2130ShadowRegs.TPWMTHRS = data;
                break;
            case DRV_REG_TCOOLTHRS:
                TMC2130ShadowRegs.TCOOLTHRS = data;
                break;
            case DRV_REG_THIGH:
                TMC2130ShadowRegs.THIGH = data;
                break;
            default:
                break;
        }
        return success;
    }
    
    //TODO: handle status bits in result
    return 0;
}

void setMotorDirection(enum SPIDest drv, bool invert){
    union TMC2130_GCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_GCONF, &(data.all));
    data.shaft = invert;
    TMC2130_write_reg(drv, DRV_REG_GCONF, data.all);
}
    
void setHoldCurrent(enum SPIDest drv, unsigned char current){
    //Need to use shadow regs for this
    TMC2130ShadowRegs.IHOLD_IRUN.IHOLD = current;
    TMC2130_write_reg(drv, DRV_REG_IHOLD_IRUN, TMC2130ShadowRegs.IHOLD_IRUN.all);
}

void setRunCurrent(enum SPIDest drv, unsigned char current){
    //Need to use shadow regs for this
    TMC2130ShadowRegs.IHOLD_IRUN.IRUN = current;
    TMC2130_write_reg(drv, DRV_REG_IHOLD_IRUN, TMC2130ShadowRegs.IHOLD_IRUN.all);
}

void setHoldCurrentRampTime(enum SPIDest drv, unsigned char time){
    //Need to use shadow regs for this
    TMC2130ShadowRegs.IHOLD_IRUN.IHOLDDELAY = time;
    TMC2130_write_reg(drv, DRV_REG_IHOLD_IRUN, TMC2130ShadowRegs.IHOLD_IRUN.all);
}

void setHoldDelay(enum SPIDest drv, unsigned char delay){
    //Need to use shadow regs for this
    TMC2130ShadowRegs.TPOWERDOWN = delay;
    TMC2130_write_reg(drv, DRV_REG_TPOWERDOWN, TMC2130ShadowRegs.TPOWERDOWN);
}

void setDoubleEdgeStep(enum SPIDest drv, bool enable){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.dedge = enable;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setMicrostepInterpolation(enum SPIDest drv, bool enable){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.intpol = enable;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setMicrostepping(enum SPIDest drv, unsigned char microstepping){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.mres = microstepping;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setCurrentSensitivity(enum SPIDest drv, bool sensitivity){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.vsense = sensitivity;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setComparatorBlankTime(enum SPIDest drv, unsigned char blankTime){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.tbl = blankTime;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setChopperMode(enum SPIDest drv, bool constantOff){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.chm = constantOff;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}

void setTOFFTime(enum SPIDest drv, unsigned char time){
    union TMC2130_CHOPCONF_t data;
    TMC2130_read_reg(drv, DRV_REG_CHOPCONF, &(data.all));
    data.toff = time;
    TMC2130_write_reg(drv, DRV_REG_CHOPCONF, data.all);
}