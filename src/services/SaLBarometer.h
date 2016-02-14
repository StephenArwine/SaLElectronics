
#ifndef SALBAROMETER_H_
#define SALBAROMETER_H_

#include <SaLTypes.h>
#include <SaLSensor.h>
#include <boardDefines.h>
#include <SaLPort.h>
#include <MS5607.h>
#include <SaLDelay.h>
#include <SaLClocks.h>

bool baroSample(void);

enum baroSampleState {
    baroSampleEmpty = 0,
    baroSamplePressureRequested = 1,
    baroSamplePressureReady = 2,
    baroSamplePressureRetreaved = 3,
    baroSampleTemperatureRequested = 4,
    baroSampleTemperatureReady = 5,
    baroSampleTemperatureRetreaved = 6,
    baroSamplePascelCalculated = 7,
    baroSampleHeightCalculated = 8
};



struct BarometerModule {

#ifdef HAS_MS5607

    uint32_t AdcReadResult_;
    uint32_t temperature;
    uint32_t pressure;

#endif

    uint32_t currentAltInFt;

};

uint32_t SaLBaroGetHeight();

void initBarometer();

#ifdef HAS_MS5607

void sendBaroAdcReadRequest();
void getBaroAdcRead(struct BarometerModule *const myBarometer);
void sendMS5607D2ReadReq(uint8_t _cmd);
void getMS5607PressureSlow(struct BarometerModule *const myBarometer);



extern enum baroSampleState baroSampleState;
extern uint16_t coefficients_[6];
extern uint32_t pressure;
extern uint32_t temperature;
extern uint32_t pascelFromPresTempConv;
extern uint32_t currentAltinCm;
extern uint32_t mills;

#endif


#endif
