
#ifndef SALSAMPLE_H_
#define SALSAMPLE_H_

#include <SaLTypes.h>
#include <math.h>

#include <SaLAltimeter.h>
#include <SaLAccelerometer.h>
#include <SaLBarometer.h>
#include <SaLGps.h>

struct sample {

    uint32_t pressure;
    float altitude;
    float groundLevel;
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
	float position;
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