

#ifndef SALALTIMETER_H_
#define SALALTIMETER_H_

//#include <SaL.h>
#include <SaLAccelerometer.h>
#include <SaLBarometer.h>
#include <SaLGps.h>

struct AltimeterModule {

    struct AccelerometerModule *myAltimetersAccelerometer;

    struct BarometerModule *myAltimetersBarometer;

    struct GpsModule *myAltimetersGps;

    float groundHeight;


};





#endif