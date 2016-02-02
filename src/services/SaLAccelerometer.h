
#ifndef SALACCELEROMETER_H_
#define SALACCELEROMETER_H_

#include <SaLTypes.h>
#include <ADXL345.h>
#include <SaLConv2C.h>


struct AccelerometerModule;

struct AccelerationValues {
    int16_t X;
    int16_t Y;
    int16_t Z;
    float Xf;
    float Yf;
    float Zf;
};


struct AccelerometerModule {
    ///  enum model_;

#ifdef HAS_ADXL345

    uint8_t range_;

    uint8_t rate_;

#endif

    struct AccelerationValues acceleration;

};

enum accelSampleState {
    accelSampleEmpty = 0,
    accelSampleretreaved = 1,
    accelSampleXIntConverted = 2,
    accelSampleYIntConverted = 3,
    accelSampleZIntConverted = 4,
    accelSampleXFloatConverted = 5,
    accelSampleYFloatConverted = 6,
    accelSapleCooked = 7
};
extern enum accelSampleState accelSampleState;

bool AccelSample;
extern uint8_t regAccelData[6];
extern int16_t X;
extern int16_t Y;
extern int16_t Z;
extern float Xf;
extern float Yf;
extern float Zf;



void getAccelEvent(struct AccelerometerModule *myAccelerometer);

void initAccelerometer(struct AccelerometerModule *myAccelerometer);

float SaLGetAccelX();
float SaLGetAccelY();
float SaLGetAccelZ();


#endif