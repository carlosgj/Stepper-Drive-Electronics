#include <xc.h>
#include "TMC429.h"

unsigned char TMC429_read_reg(unsigned char addr, uint24_t *data){
    TMC429_Tx_Datagram datagram;
    TMC429_Rx_Datagram result;
    datagram.read = TRUE;
    datagram.RRS = FALSE;
    datagram.addr = addr;
    datagram.data.all = 0;
    
    SPIXfer(MC, datagram.bytes, result.bytes, 4);
    
    //TODO: handle status bits in result
}

unsigned char TMC429_write_reg(unsigned char addr, uint24_t data){
    TMC429_Tx_Datagram datagram;
    TMC429_Rx_Datagram result;
    datagram.read = FALSE;
    datagram.RRS = FALSE;
    datagram.addr = addr;
    datagram.data.all = data;
    
    SPIXfer(MC, datagram.bytes, result.bytes, 4);
    
    //TODO: handle status bits in result
}