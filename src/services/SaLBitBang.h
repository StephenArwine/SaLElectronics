



#ifndef SALBITBANG_H_
#define SALBITBANG_H_

#include <SaL.h>


void byteOut(struct spiModule *const module,
             uint8_t btye);

void bitsOut(struct spiModule *const module,
             uint8_t dataBits,uint8_t bits);

uint8_t getByte(struct spiModule *const module);

uint16_t getBits(struct spiModule *const module,uint8_t bits);


#endif


