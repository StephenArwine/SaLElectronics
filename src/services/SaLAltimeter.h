

#ifndef SALALTIMETER_H_
#define SALALTIMETER_H_

#include <SaLAccelerometer.h>
#include <SaLBarometer.h>
#include <SaLGps.h>

struct AltimeterModule;

struct AltimeterModule {

    struct AccelerometerModule *myAltimetersAccelerometer;

    struct BarometerModule *myAltimetersBarometer;

    struct GpsModule *myAltimetersGps;

    float groundHeight;


};





#endif