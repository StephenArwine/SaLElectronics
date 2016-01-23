
#ifndef SALACCELEROMETER_H_
#define SALACCELEROMETER_H_

#include <SaL.h>


struct Acceleration {
    int16_t X;
    int16_t Y;
    int16_t Z;
    float Xf;
    float Yf;
    float Zf;
};


struct Accelerometer {

  ///  enum model_;

#ifdef HAS_ADXL345

    uint8_t range_;

    uint8_t rate_;

#endif

    struct Acceleration acceleration;


};


void getAccelEvent(struct Accelerometer *const myAccelerometer);

void initAccelerometer(
    struct Accelerometer *const myAccelerometer);


#endif