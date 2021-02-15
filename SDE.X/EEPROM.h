#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> 
#include <string.h>
#include "common.h"

#define EEP_ERR_SUCCESS     (0)
#define EEP_ERR_WTIMEOUT    (1)
#define EEP_ERR_READBACK    (2)
#define EEP_ERR_COMPLEMENT  (3)
#define EEP_ERR_INVAL_ADR   (4)

#define EEP_ERR_LEN (3)

union EEPError_t {
    unsigned char all[EEP_ERR_LEN];
    struct {
        unsigned char writeTimeout;
        unsigned char readback;
        unsigned char complement;
    };
} EEPErrors;

void EEP_Init(void);
unsigned char EEP_Read(unsigned char address, unsigned char len, unsigned char *dest);
unsigned char EEP_Write(unsigned char address, unsigned char len, unsigned char *data);
unsigned char EEP_ReadComp(unsigned char address, unsigned char *dest);
unsigned char EEP_WriteComp(unsigned char address, unsigned char data);
unsigned char EEP_WriteByte(unsigned char address, unsigned char data);
unsigned char EEP_ReadByte(unsigned char address);

#endif

