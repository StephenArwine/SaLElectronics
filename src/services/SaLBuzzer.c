
#include <SaLBuzzer.h>


void SaLPlayTone(int16_t tone_,int16_t durration) {


    int32_t elapsed_time = 0;


    while (elapsed_time < (durration*100)) {

        SaLDigitalOut(BUZZER,true);
        delay_us(tone_/2);

        SaLDigitalOut(BUZZER, false);
        delay_us(tone_/2);


        elapsed_time += (tone_);



    }


}