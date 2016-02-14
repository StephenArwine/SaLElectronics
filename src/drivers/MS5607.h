

#ifndef MS5607_H_
#define MS5607_H_

#include <SaLTypes.h>
#include <boardDefines.h>
#include <SaLPort.h>
#include <SaLBitBang.h>
#include <SaLDelay.h>

#define MS5607_CMD_RES    0x1E
#define cmdAdcRead_  0x00
#define cmdAdcConv_  0x40
#define cmdAdcD1_    0x40
#define cmdAdcD2_    0x50
#define cmdAdc256_   0x00
#define cmdAdc512_   0x02
#define cmdAdc1024_  0x04
#define cmdAdc2048_  0x06
#define cmdAdc4096_  0x08
#define MS5607_CMD_PROM_READ  0xA0
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

uint32_t ConvertPressureTemperature(uint32_t *pressure,
                                    uint32_t *temperature,
                                    uint16_t *coefficients);

uint32_t getAltFt();
uint32_t pascalToCent(const int32_t pressurePa);

float paToFeetNOAA(const int32_t pressurePa);

void getMS5607Coeff(uint16_t *coefficients);

uint32_t readMS5607AdcResults();

#endif /* MS5607_H_ */

