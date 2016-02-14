
#ifndef SALSAMPLE_H_
#define SALSAMPLE_H_

#include <SaLTypes.h>
#include <math.h>

#include <SaLAltimeter.h>
#include <SaLAccelerometer.h>
#include <SaLBarometer.h>
#include <SaLGps.h>
#include <SaLDelay.h>
#include <SaLBuzzer.h>

struct sample {

    uint32_t pressure;
    uint32_t altitude;
    uint32_t groundLevel;
    float accelerationInX;
    float accelerationInY;
    float accelerationInZ;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float longitude;
    float latitude;
    bool GPSvalid;
    uint32_t sampleMills;
    uint32_t deltaT;
    float positionStar;
    float velocityStar;
    uint32_t position;
    float velocity;

};

extern bool baroSampleReady;
extern bool accelSampleReady;
extern bool gpsSampleReady;
extern struct sample sampleBeingCooked;
extern struct sample cookedSample;

void sampleInit();

/* returns a current data sample from the altimeters sensors */
struct sample getSample(struct AltimeterModule altimeter);

void sampleTick();

#endif