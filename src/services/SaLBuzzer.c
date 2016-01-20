
#include <SaLBuzzer.h>


void SaLPlayTone(int16_t tone_) {

	
	int32_t elapsed_time = 0;
	int32_t duration = 100000;
	
    while (elapsed_time < duration) {
		
		SaLDigitalOut(BUZZER,true);
		delay_us(tone_/2);
		
		 SaLDigitalOut(BUZZER, false);
		 delay_us(tone_/2);
		
	
	      elapsed_time += (tone_);

	
	
	}


}