
#ifndef SALBAROMETER_H_
#define SALBAROMETER_H_

#include <SaLTypes.h>
#include <SaLSensor.h>
#include <boardDefines.h>
#include <SaLPort.h>

#include <MS5607.h>



struct BarometerModule;

struct BarometerModule {

#ifdef HAS_MS5607

    uint32_t AdcReadResult_;

    uint16_t coefficients_[6];

    uint32_t temperature;

    uint32_t pressure;


#endif

    uint32_t currentAltInFt;


};

void initBarometer(struct BarometerModule *const myBarometer);

#ifdef HAS_MS5607

void sendBaroAdcReadRequest();

void getBaroAdcRead(struct BarometerModule *const myBarometer);

void sendMS5607D2ReadReq(uint8_t _cmd);

void getMS5607PressureSlow(struct BarometerModule *const myBarometer);

#endif


#endif
