#include <SaLSpi.h>


// void configSpiModule(struct spiModule *const module,
//                      uint8_t MOSI_PIN,
//                      uint8_t MISO_pin,
//                      uint8_t SCK_PIN,
//                      uint8_t SS_PIN ) {
//     module->MOSI		= MOSI_PIN;
//     module->MISO		= MISO_pin;
//     module->SCK		    = SCK_PIN;
//     module->SS			= SS_PIN;
// 
//     SaLPinMode(module->MOSI,OUTPUT);
//     SaLPinMode(module->SCK ,OUTPUT);
//     SaLPinMode(module->SS,  OUTPUT);
//     SaLPinMode(module->MISO,INPUT);
//     SaLDigitalOut(module->SS,true);
// 
//     module->BYTEIN		= 0x00;
//     module->BYTEOUT		= 0x00;
// }