#include <xc.h>
#include "TMC2130.h"

void TMC2130Periodic(void){
    static unsigned char counter = 10;
    if(counter-- != 0){
        return;
    }
    
    counter = 10;
}

unsigned char TMC2130_read_reg(enum SPIDest target, unsigned char addr, uint32_t *data){
    
}

unsigned char TMC2130_write_reg(enum SPIDest target, unsigned char addr, uint32_t data){
    
}