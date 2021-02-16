#include <xc.h>
#include "TMC429.h"

void TMC429Init(void){
    //Initialize motor status
    memset(M1Stat.all, 0, MOTOR_STAT_LEN);
    memset(M2Stat.all, 0, MOTOR_STAT_LEN);
    memset(M3Stat.all, 0, MOTOR_STAT_LEN);
    
    //Check TMC429 version register
    uint24_t version;
    TMC429_read_reg(MC_PREFIX_COMMON | MC_COMMON_VERSION, &version);
    if(version == 0x429101){
        systStat.TMC429Stat = TMC429_GOOD;
    }
    else{
        systStat.TMC429Stat = TMC429_BAD_VERSION;
        return;
    }
    
    //Configure basic stuff
    union MC_IFCONFIG_t ifconfigVal;
    ifconfigVal.all = 0;
    ifconfigVal.en_sd = TRUE; //Step/dir mode
    TMC429_write_reg(MC_PREFIX_COMMON | MC_COMMON_IFCONFIG, ifconfigVal.all);
    
    TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_TARGET, 0);
    TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_ACTUAL, 0);
    TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_TARGET, 0);
    TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_ACTUAL, 0);
    TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_TARGET, 0);
    TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_ACTUAL, 0);
}

void TMC429Periodic(void){
    static unsigned char counter = 10;
    if(counter-- != 0){
        return;
    }
    TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_TARGET, &M1Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_ACTUAL, &M1Stat.actual);
    TMC429_read_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_TARGET, &M2Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_ACTUAL, &M2Stat.actual);
    TMC429_read_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_TARGET, &M3Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_ACTUAL, &M3Stat.actual);
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