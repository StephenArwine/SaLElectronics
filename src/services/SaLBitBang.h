#include <SaL.h>


#ifndef SALBITBANG_H_
#define SALBITBANG_H_



typedef struct spiModule {

    uint8_t MISO;

    uint8_t MOSI;

    uint8_t SCK;

    uint8_t SS;

    uint8_t BYTEOUT;

    uint8_t BYTEIN;

#define PINHIGH true
#define PINLOW false


} spiModule;

static inline void configSpiModule(struct spiModule *const module,
                                   uint8_t MOSI_PIN,
                                   uint8_t MISO_pin,
                                   uint8_t SCK_PIN,
                                   uint8_t SS_PIN )

{
    module->MOSI		= MOSI_PIN;
    module->MISO		= MISO_pin;
    module->SCK		    = SCK_PIN;
    module->SS			= SS_PIN;

    SaLPinMode(module->MOSI,OUTPUT);
    SaLPinMode(module->SCK ,OUTPUT);
    SaLPinMode(module->SS,  OUTPUT);
    SaLPinMode(module->MISO,INPUT);
    SaLDigitalOut(module->SS,true);



    module->BYTEIN		= 0x00;
    module->BYTEOUT		= 0x00;
};

void byteOut(struct spiModule *const module,
             uint8_t btye);
			 
void bitsOut(struct spiModule *const module,
             uint8_t dataBits,uint8_t bits);

uint8_t getByte(struct spiModule *const module);
uint16_t getBits(struct spiModule *const module,uint8_t bits);


#endif


