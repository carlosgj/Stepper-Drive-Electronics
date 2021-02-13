#include <xc.h>
#include "TMC429.h"

void TMC429Periodic(void){
    static unsigned char counter = 10;
    if(counter-- != 0){
        return;
    }
    
    counter = 10;
}

unsigned char TMC429_read_reg(unsigned char addr, uint24_t *data){
    unsigned char success;
    TMC429_Tx_Datagram datagram;
    TMC429_Rx_Datagram result;
    datagram.read = TRUE;
    datagram.RRS = FALSE;
    datagram.addr = addr;
    datagram.data = 0;
    
    success = SPIXfer(MC, datagram.bytes, result.bytes, 4);
    
    if(success != 0){
        return success;
    }
    
    //TODO: handle status bits in result
    
    *data = result.data;
    
    return 0;
}

unsigned char TMC429_write_reg(unsigned char addr, uint24_t data){
    unsigned char success;
    TMC429_Tx_Datagram datagram;
    TMC429_Rx_Datagram result;
    datagram.read = FALSE;
    datagram.RRS = FALSE;
    datagram.addr = addr;
    datagram.data = data;
    
    success = SPIXfer(MC, datagram.bytes, result.bytes, 4);
    
    if(success != 0){
        return success;
    }
    
    //TODO: handle status bits in result
    
    return 0;
}