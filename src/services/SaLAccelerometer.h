
#ifndef SALACCELEROMETER_H_
#define SALACCELEROMETER_H_

#include <SaL.h>


struct AccelerationValues {
    int16_t X;
    int16_t Y;
    int16_t Z;
    float Xf;
    float Yf;
    float Zf;
};


typedef struct AccelerometerModule {
    ///  enum model_;

#ifdef HAS_ADXL345

    uint8_t range_;

    uint8_t rate_;

#endif

   struct AccelerationValues acceleration;


} Accelerometer, *AccelerometerPtr;


void getAccelEvent(Accelerometer *const myAccelerometer);

void initAccelerometer(Accelerometer *const myAccelerometer);


#endif