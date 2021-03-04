#include <xc.h>
#include "TMC2130.h"

void TMC2130Init(void){
    //Verify ID reg
    uint32_t data;
    
    TMC2130_read_reg(DRV1, DRV_REG_GSTAT, &data);
    TMC2130_read_reg(DRV1, DRV_REG_IOIN, &data);
    if((data >> 24) == 0x11){
        M1Stat.TMC2130Stat = TMC2130_GOOD;
    }
    else{
        M1Stat.TMC2130Stat = TMC2130_BAD_VERSION;
    }
    
    TMC2130_read_reg(DRV1, DRV_REG_GSTAT, &data);
    TMC2130_read_reg(DRV2, DRV_REG_IOIN, &data);
    if((data >> 24) == 0x11){
        M2Stat.TMC2130Stat = TMC2130_GOOD;
    }
    else{
        M2Stat.TMC2130Stat = TMC2130_BAD_VERSION;
    }
    
    TMC2130_read_reg(DRV1, DRV_REG_GSTAT, &data);
    TMC2130_read_reg(DRV3, DRV_REG_IOIN, &data);
    if((data >> 24) == 0x11){
        M3Stat.TMC2130Stat = TMC2130_GOOD;
    }
    else{
        M3Stat.TMC2130Stat = TMC2130_BAD_VERSION;
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
        return success;
    }
    
    //TODO: handle status bits in result
    return 0;
}