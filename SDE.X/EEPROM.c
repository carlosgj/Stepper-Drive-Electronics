#include "EEPROM.h"
#include "commConstants.h"

void EEP_Init(void){
    EECON1bits.WREN = FALSE;
    
    memset(EEPErrors.all, 0, EEP_ERR_LEN);
}

unsigned char EEP_Read(unsigned char address, unsigned char len, unsigned char *dest){
    if((address+len) > 127){
        return EEP_ERR_INVAL_ADR; //TODO
    }
    unsigned char status = EEP_ERR_SUCCESS;
    unsigned char i=0; 
    for(i=0; i<len; i++){
        status = EEP_ReadComp(address++, &dest[i]);
        if(status != EEP_ERR_SUCCESS){
            return status;
        }
    }
    return status;
}

unsigned char EEP_Write(unsigned char address, unsigned char len, unsigned char *data){
    if((address+len) > 127){
        return EEP_ERR_INVAL_ADR; //TODO
    }
    unsigned char status = EEP_ERR_SUCCESS;
    unsigned char i=0; 
    for(i=0; i<len; i++){
        status = EEP_WriteComp(address++, data[i]);
        if(status != EEP_ERR_SUCCESS){
            return status;
        }
    }
    return status;
}

unsigned char EEP_ReadComp(unsigned char address, unsigned char *dest){
    unsigned char compAddr = (unsigned char)(address + 128);
    //Read normal data
    unsigned char data = EEP_ReadByte(address);
    //Read complement data
    unsigned char compData = EEP_ReadByte(compAddr);
    
    if(data != ~compData){
        //Complement error
        EEPErrors.complement++;
        return EEP_ERR_COMPLEMENT;
    }
    
    *dest = data;
    return EEP_ERR_SUCCESS;
}

unsigned char EEP_WriteComp(unsigned char address, unsigned char data){
    unsigned char compAddr = (unsigned char)(address + 128);
    unsigned char compData = (unsigned char)(~data);
    
    unsigned char result = EEP_ERR_SUCCESS;
    result = EEP_WriteByte(address, data);
    if(result != EEP_ERR_SUCCESS){
        return result;
    }
    result = EEP_WriteByte(compAddr, compData);
    return result;
}

unsigned char EEP_WriteByte(unsigned char address, unsigned char data){
    EEADRL = address;
    EEDATL = data;
    PIR2bits.EEIF = FALSE;
    EECON1bits.WREN = TRUE;
    
    INTDIS;
    EECON2 = 0x55;
    EECON2 = 0xaa;
    EECON1bits.WR = TRUE;
    INTEN;
    
    //Wait for write to finish
    unsigned char i = 255; //TODO
    while(i>0){
        i--;
        if(PIR2bits.EEIF){
            //Write finished
            break;
        }
    }
    
    EECON1bits.WREN = FALSE;
    
    if(!PIR2bits.EEIF){
        //Timeout
        EEPErrors.writeTimeout++;
        return EEP_ERR_WTIMEOUT;
    }
    
    //Do readback to verify write
    unsigned char readback = EEP_ReadByte(address);
    if(readback != data){
        //Readback error
        EEPErrors.readback++;
        return EEP_ERR_READBACK;
    }
    
    return EEP_ERR_SUCCESS;
}

unsigned char EEP_ReadByte(unsigned char address){
    EEADRL = address;
    EECON1bits.CFGS = FALSE;
    EECON1bits.EEPGD = FALSE;
    EECON1bits.RD = TRUE;
    return EEDATL;
}