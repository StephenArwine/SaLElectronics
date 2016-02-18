



#ifndef SALBITBANG_H_
#define SALBITBANG_H_

#include <boardDefines.h>
#include <SaLTypes.h>
#include <SaLPort.h>

uint8_t syncByte(Pin SCK_PIN,
                 Pin MOSI_PIN,
                 Pin MISO_PIN,
                 uint8_t byte);

void byteOut(uint8_t SCK_PIN,
             uint8_t MOSI_PIN,
             uint8_t byte);

void bitsOut(uint8_t SCK_PIN,
             uint8_t MOSI_PIN,
             uint8_t dataBits,
             uint8_t bits);

uint8_t getByte(uint8_t SCK_PIN,
                uint8_t MISO_PIN);

uint16_t getBits(uint8_t SCK_PIN,
                 uint8_t MISO_PIN,
                 uint8_t bits);


#endif


