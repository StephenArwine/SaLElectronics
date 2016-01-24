
#ifndef SALBAROMETER_H_
#define SALBAROMETER_H_

#include <SaL.h>


typedef struct BarometerModule {

#ifdef HAS_MS5607

uint32_t AdcReadResult_;

uint16_t coefficients_[6];

uint32_t temperature;

uint32_t pressure;

#endif

uint32_t currentAltInFt;


}Barometer, *BarometerPtr;

void initBarometer(Barometer *const myBarometer);

#ifdef HAS_MS5607

void sendBaroAdcReadRequest();

void getBaroAdcRead(Barometer *const myBarometer);

void sendMS5607D2ReadReq(uint8_t _cmd);

void getMS5607PressureSlow(Barometer *const myBarometer);

#endif





#endif