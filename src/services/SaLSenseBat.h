#ifndef SALBATSENSE_H_
#define SALBATSENSE_H_

#include <SaLTypes.h>
#include <SaLAnalog.h>
#include <SaLPort.h>
#include <boardDefines.h>

#define batR1 100
#define batR2 27

#ifdef SaLFrostburn1_1
#endif


static inline float senseBatVolts(Pin p) {
   // float VoltageDivider = ((100+27)/27);

     return ((3.60/2.0)/pow(2,12) * 4.703) * adcRead(p);
//return adcRead(p);
}


#endif