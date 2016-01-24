

#ifndef SALALTIMETER_H_
#define SALALTIMETER_H_

#include <SaL.h>

typedef struct AltimeterModule {

   struct AltimeterPtr *myAltimetersAccelerometer;

    struct BarometerPtr *myAltimetersBarometer;


} Altimeter, *AltimeterPtr;





#endif