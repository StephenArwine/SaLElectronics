

#ifndef SALBUZZER_H_
#define SALBUZZER_H_


#include <SaLPort.h>
#include <SaLDelay.h>
#include <boardDefines.h>

void SaLBuzzerInit();

void SaLPlayTone(int16_t tone_,int16_t durration);

static inline void startUpTone() {
    SaLPlayTone(900,300);
    SaLPlayTone(800,300);
    SaLPlayTone(700,300);
    SaLPlayTone(600,300);
    SaLPlayTone(500,300);

    SaLPlayTone(900,300);
    SaLPlayTone(800,300);
    SaLPlayTone(700,300);
    SaLPlayTone(600,300);
    SaLPlayTone(500,300);
};


void toneTick();


#endif