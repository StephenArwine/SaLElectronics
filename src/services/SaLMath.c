
#include <SaLMath.h>


void addSampleToVariance(struct sVar *var, uint32_t newSample) {
    int16_t delta = newSample - var->mean;
    var->numSamples++;
    var->mean += delta/var->numSamples;
    var->M2 += delta * (newSample - var->mean);
}

uint32_t GetVariance(struct sVar *var, uint32_t *average) {
    uint16_t variance = var->M2/var->numSamples;
    *average = var->mean;

    var->numSamples = 0;
    var->mean = 0;
    var->M2 = 0;

    return variance;
}