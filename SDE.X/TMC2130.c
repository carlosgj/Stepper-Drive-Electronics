#include <xc.h>
#include "TMC2130.h"

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
    datagram.data.all = 0;
    
    success = SPIXfer(MC, datagram.bytes, result.bytes, 5);
    
    if(success != 0){
        return success;
    }
    
    //TODO: handle status bits in result
    
    *data = result.data.all;
    return 0;
}

unsigned char TMC2130_write_reg(enum SPIDest target, unsigned char addr, uint32_t data){
    unsigned char success;
    TMC2130_Tx_Datagram datagram;
    TMC2130_Rx_Datagram result;
    datagram.write = TRUE;
    datagram.addr = addr;
    datagram.data.all = data;
    
    success = SPIXfer(MC, datagram.bytes, result.bytes, 4);
    
    if(success != 0){
        return success;
    }
    
    //TODO: handle status bits in result
    return 0;
}