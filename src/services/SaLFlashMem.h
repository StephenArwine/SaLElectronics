
#ifndef SALFLASHMEM_H_
#define SALFLASHMEM_H_

#include <SaLSample.h>
#include <SaLTypes.h>
#include <SaLPort.h>

void SaLFlashMemInit();

uint32_t SaLWriteSampleToFlashMemory(struct sample sample);


#endif