#ifndef ADC_H
#define	ADC_H

#include <xc.h>
#include "common.h"

#define ADC_STAT_SUCCESS (0)
#define ADC_STAT_TIMEOUT (1)

#define ADC_CONV_TIMEOUT (255) //TODO

#define ADCH_AN0 (0b00000)
#define ADCH_AN1 (0b00001)
#define ADCH_AN2 (0b00010)
#define ADCH_AN3 (0b00011)
#define ADCH_AN4 (0b00100)
#define ADCH_AN5 (0b00101)
#define ADCH_AN6 (0b00110)
#define ADCH_AN7 (0b00111)
#define ADCH_AN8 (0b01000)
#define ADCH_AN9 (0b01001)
#define ADCH_AN10 (0b01010)
#define ADCH_AN11 (0b01011)
#define ADCH_TEMP (0b11101)
#define ADCH_DAC (0b11110)
#define ADCH_FVR (0b11111)

union ADC_Res_t{
    uint16_t all;
    struct{
        unsigned char low;
        unsigned char high;
    };
};

void ADC_Init(void);

unsigned char takeMeasurement(unsigned char channel, unsigned char numReadings, uint16_t *result);

#endif

