#ifndef SALBATSENSE_H_
#define SALBATSENSE_H_

#include <SaLTypes.h>
#include <SaLAnalog.h>
#include <SaLPort.h>

static inline float senseBatVolts(Pin p){
	return adcRead(p) * 0.0293;
}


#endif