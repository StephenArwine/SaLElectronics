
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


void getAccelEvent(struct AccelerometerModule *myAccelerometer);

void initAccelerometer(struct AccelerometerModule *myAccelerometer);


#endif