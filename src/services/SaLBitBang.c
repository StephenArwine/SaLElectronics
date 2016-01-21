
#include "SaLBitBang.h"



void byteOut(struct spiModule *const module,
             uint8_t byte) {

    delay_us(90);
    for (uint8_t i = 0; i<8; ++i) {
        SaLDigitalOut(module->SCK,PINLOW);
        delay_us(90);
        if (byte & 0x80) {
            SaLDigitalOut(module->MOSI,true);
            delay_us(90);
        } else {
            SaLDigitalOut(module->MOSI,false);
            delay_us(90);
        }
        SaLDigitalOut(module->SCK,PINHIGH);
        delay_us(90);
        byte <<= 1;
    }

}

void bitsOut(struct spiModule *const module,
             uint8_t dataBits,uint8_t bits) {

    for (uint8_t i = 0; i<bits; ++i) {
        SaLDigitalOut(module->SCK,PINLOW);
        if (dataBits & 0x80) {
            SaLDigitalOut(module->MOSI,true);
        } else {
            SaLDigitalOut(module->MOSI,false);
        }
        SaLDigitalOut(module->SCK,PINHIGH);
        dataBits <<= 1;
    }

}

uint8_t getByte(struct spiModule *const module) {
    uint8_t byte = 0x00;
    delay_us(90);
    for (uint8_t i = 0; i<8; ++i) {
        SaLDigitalOut(module->SCK,PINLOW);
        delay_us(90);
        byte = (byte << 1 ) | SaLDigitalIn(module->MISO);
        delay_us(90);
        SaLDigitalOut(module->SCK,PINHIGH);
        delay_us(90);
    }
    return byte;
}

uint16_t getBits(struct spiModule *const module,uint8_t bits) {
    uint16_t data = 0x00;
    delay_us(90);
    for (uint8_t i = 0; i<bits; ++i) {
        SaLDigitalOut(module->SCK,PINLOW);
        delay_us(90);
        data = (data << 1 ) | SaLDigitalIn(module->MISO);
        delay_us(90);
        SaLDigitalOut(module->SCK,PINHIGH);
        delay_us(90);
    }
    return data;
}