

#ifndef MS5607_H_
#define MS5607_H_

#include <SaL.h>


#define cmdReset_    0x1E
#define cmdAdcRead_  0x00
#define cmdAdcConv_  0x40
#define cmdAdcD1_    0x40
#define cmdAdcD2_    0x50
#define cmdAdc256_   0x00
#define cmdAdc512_   0x02
#define cmdAdc1024_  0x04
#define cmdAdc2048_  0x06
#define cmdAdc4096_  0x08
#define cmdPromRd_   0xA0
uint16_t coefficients_[6];

struct MS5607_module {

    uint8_t sercom1setting;

    uint8_t sercom2Setting;

    uint8_t SSPin;

    uint16_t coefficients_[6];

    uint32_t rawPressureData;

    uint32_t rawTempatureDate;

    uint32_t tempatureCelcus;

    uint32_t pressurePa;

    uint32_t heightFeet;


};

uint32_t read_Adc(
                  uint8_t _cmd);

void read_coeff();

uint32_t ConvertPressureTemperature(uint32_t pressure,
                                    uint32_t temperature);

uint32_t getAltFt();
uint32_t pascalToCent(const int32_t pressurePa);

uint32_t paToFeetNOAA(const int32_t pressurePa);

// int16_t getX(struct spiModule *const module);
// int16_t getY(struct spiModule *const module);
// int16_t getZ(struct spiModule *const module);

#endif /* MS5607_H_ */

