

#ifndef SALBUZZER_H_
#define SALBUZZER_H_
#define BUZZER PIN_PA12

#include <SaL.h>

void SaLPlayTone(int16_t tone_);

static inline void startUpTone() {
    SaLPlayTone(900);
    SaLPlayTone(800);
    SaLPlayTone(700);
    SaLPlayTone(600);
    SaLPlayTone(500);
};




#endif