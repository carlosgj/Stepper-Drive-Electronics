#include <xc.h>
#include "TMC429.h"

void TMC429Init(void){
    //Initialize motor status
    memset(M1Stat.all, 0, MOTOR_STAT_LEN);
    memset(M2Stat.all, 0, MOTOR_STAT_LEN);
    memset(M3Stat.all, 0, MOTOR_STAT_LEN);
    
    M1Stat.homeStat = MS_IDLE;
    M2Stat.homeStat = MS_IDLE;
    M3Stat.homeStat = MS_IDLE;
    
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
    
    //Test settings
    TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_VMIN, 1);
    TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_VMAX, 10);
    TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_AMAX, 20);
    TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_VMIN, 1);
    TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_VMAX, 10);
    TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_AMAX, 20);
    TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_VMIN, 1);
    TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_VMAX, 10);
    TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_AMAX, 20);
}

void TMC429Periodic(void){
    static unsigned char counter = TMC429_PERIOD;
    if(counter-- != 0){
        return;
    }
    TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_TARGET, &M1Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_ACTUAL, &M1Stat.actual);
    TMC429_read_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_TARGET, &M2Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_ACTUAL, &M2Stat.actual);
    TMC429_read_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_TARGET, &M3Stat.target);
    TMC429_read_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_ACTUAL, &M3Stat.actual);
    
    if(M1Stat.homeStat == MS_HOMING){
        TMC429Homing(DRV1);
//        switch(M1HomeStep){
//            case HS_INIT:
//                //If currently moving, stop moving
//                if(!M1Stat.onTarget){
//                    setTargetPos(DRV1, M1Stat.actual);
//                    break;
//                }
//                //Record current position for homing difference calculation
//                M1HomingDifference = M1Stat.actual;
//                if(M1Stat.leftLimit){
//                    //If we're already on the limit switch, we need to move off
//                    //Assume we're at 0, and move forward some 
//                    //TODO: move portion of rev
//                    setActualPos(DRV1, 0);
//                    setTargetPos(DRV1, 1000);
//                    M1HomeStep = HS_MOVEOFF;
//                }
//                else{
//                    //If not, move on to the next step
//                    M1HomeStep = HS_ISOFF;
//                }
//                break;
//            case HS_MOVEOFF:
//                //Check if the move-away is done
//                if(M1Stat.onTarget){
//                    //Verify we're not still on the switch
//                    if(M1Stat.leftLimit){
//                        //If so, this is an error
//                        //TODO: EVR?
//                        M1Stat.homeStat = MS_HOME_ERR;
//                    }
//                    else{
//                        //Move on to the next step
//                        M1HomeStep = HS_ISOFF;
//                    }
//                }
//                break;
//            case HS_ISOFF:
//                //We're now off the limit switch; prepare to move towards it
//                //Assume we're at full-right position
//                setActualPos(DRV1, 0xffffff);
//                //Prime X_Latched
//                TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_LATCHED, 0);
//                //Head towards 0
//                setTargetPos(DRV1, 0);
//                M1HomeStep = HS_MOVETO;
//                break;
//            case HS_MOVETO:
//                //We're basically just waiting to hit the switch here
//                if(M1Stat.leftLimit){
//                    TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_LATCHED, &M1TrigPos);
//                    //Go back to trigger position, in case we overshot
//                    setTargetPos(DRV1, M1TrigPos);
//                    M1HomeStep = HS_MOVEBACK;
//                }
//                break;
//            case HS_MOVEBACK:
//                //Wait until not moving
//                if(M1Stat.onTarget){
//                    //Set this as zero position
//                    setTargetPos(DRV1, 0);
//                    setActualPos(DRV1, 0);
//                    M1HomingDifference = ((uint24_t)0xffffff - M1TrigPos)-M1HomingDifference;
//                    M1Stat.homeStat = MS_HOMED;
//                }
//                break;
//            default:
//                //Should be impossible
//                break;
//        }
    }
    if(M2Stat.homeStat == MS_HOMING){
        TMC429Homing(DRV2);
    }
    if(M3Stat.homeStat == MS_HOMING){
        TMC429Homing(DRV3);
    }
    counter = TMC429_PERIOD;
}

inline void TMC429Homing(enum SPIDest motor){
    union MotorStatus_t *mstat;
    uint24_t *homeDiff;
    uint24_t *trigPos;
    enum MotorHomingStep *step;
    //Setup state variables
    switch(motor){
        case DRV1:
            mstat = &M1Stat;
            homeDiff = &M1HomingDifference;
            trigPos = &M1TrigPos;
            step = &M1HomeStep;
            break;
        case DRV2:
            mstat = &M2Stat;
            homeDiff = &M2HomingDifference;
            trigPos = &M2TrigPos;
            step = &M2HomeStep;
            break;
        case DRV3:
            mstat = &M3Stat;
            homeDiff = &M3HomingDifference;
            trigPos = &M3TrigPos;
            step = &M3HomeStep;
            break;
        default:
            return;
    }
    
    if(mstat->homeStat == MS_HOMING){
        switch(*step){
            case HS_INIT:
                //If currently moving, stop moving
                if(!mstat->onTarget){
                    setTargetPos(motor, mstat->actual);
                    break;
                }
                //Record current position for homing difference calculation
                *homeDiff = mstat->actual;
                if(mstat->leftLimit){
                    //If we're already on the limit switch, we need to move off
                    //Assume we're at 0, and move forward some 
                    //TODO: move portion of rev
                    setActualPos(motor, 0);
                    setTargetPos(motor, 1000);
                    *step = HS_MOVEOFF;
                }
                else{
                    //If not, move on to the next step
                    *step = HS_ISOFF;
                }
                break;
            case HS_MOVEOFF:
                //Check if the move-away is done
                if(mstat->onTarget){
                    //Verify we're not still on the switch
                    if(mstat->leftLimit){
                        //If so, this is an error
                        //TODO: EVR?
                        mstat->homeStat = MS_HOME_ERR;
                    }
                    else{
                        //Move on to the next step
                        *step = HS_ISOFF;
                    }
                }
                break;
            case HS_ISOFF:
                //We're now off the limit switch; prepare to move towards it
                //Assume we're at full-right position
                setActualPos(motor, 0x7fffff);
                //Prime X_Latched
                primeLatched(motor);
                //Head towards 0
                setTargetPos(motor, 0);
                *step = HS_MOVETO;
                break;
            case HS_MOVETO:
                //We're basically just waiting to hit the switch here
                if(mstat->leftLimit){
                    *trigPos = getLatched(motor);
                    //Go back to trigger position, in case we overshot
                    setTargetPos(motor, *trigPos);
                    *step = HS_MOVEBACK;
                }
                break;
            case HS_MOVEBACK:
                //Wait until not moving
                if(mstat->onTarget){
                    //Set this as zero position
                    setTargetPos(motor, 0);
                    setActualPos(motor, 0);
                    *homeDiff = ((uint24_t)0x7fffff - *trigPos)-*homeDiff;
                    mstat->homeStat = MS_HOMED;
                }
                break;
            default:
                //Should be impossible
                //TODO: Error handling
                break;
        }
    }
}

inline void setTargetPos(enum SPIDest motor, uint24_t pos){
    switch(motor){
        case DRV1:
            TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_TARGET, pos);
            break;
        case DRV2:
            TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_TARGET, pos);
            break;
        case DRV3:
            TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_TARGET, pos);
        default:
            break;
    }
}

inline void setActualPos(enum SPIDest motor, uint24_t pos){
    switch(motor){
        case DRV1:
            TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_ACTUAL, pos);
            break;
        case DRV2:
            TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_ACTUAL, pos);
            break;
        case DRV3:
            TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_ACTUAL, pos);
        default:
            break;
    }
}

inline void primeLatched(enum SPIDest motor){
    switch(motor){
        case DRV1:
            TMC429_write_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_LATCHED, 0);
            break;
        case DRV2:
            TMC429_write_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_LATCHED, 0);
            break;
        case DRV3:
            TMC429_write_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_LATCHED, 0);
        default:
            break;
    }
}

inline uint24_t getLatched(enum SPIDest motor){
    uint24_t retval = 0;
    switch(motor){
        case DRV1:
            TMC429_read_reg(MC_PREFIX_MOTOR1 | MC_MOTOR_LATCHED, &retval);
            break;
        case DRV2:
            TMC429_read_reg(MC_PREFIX_MOTOR2 | MC_MOTOR_LATCHED, &retval);
            break;
        case DRV3:
            TMC429_read_reg(MC_PREFIX_MOTOR3 | MC_MOTOR_LATCHED, &retval);
        default:
            break;
    }
    return retval;
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
    
    M1Stat.leftLimit = result.rs1;
    M1Stat.onTarget = result.eqt1;
    M2Stat.leftLimit = result.rs2;
    M2Stat.onTarget = result.eqt2;
    M3Stat.leftLimit = result.rs3;
    M3Stat.onTarget = result.eqt3;
    
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