
#ifndef SALMATH_H_
#define SALMATH_H_

#include <SaLTypes.h>


struct sVar {
    uint32_t mean;
    int32_t M2;
    uint16_t numSamples;
};


void addSampleToVariance(struct sVar *var, uint32_t newSample);
uint32_t GetVariance(struct sVar *var, uint32_t *average);



#endif