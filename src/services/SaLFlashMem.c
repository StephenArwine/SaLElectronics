

#include <SaLFlashMem.h>

void SaLFlashMemInit() {

#ifdef HAS_AT25SF

    SaLPinMode(AT25SF_SLAVE_SELECT_PIN,OUTPUT);
    SaLPinMode(AT25SF_SLAVE_SELECT_PIN,TRUE);
    SaLPinMode(AT25SF_MOSI_PIN,OUTPUT);
    SaLPinMode(AT25SF_SCK_PIN,OUTPUT);
    SaLPinMode(AT25SF_MISO_PIN,INPUT);

#endif
}

uint32_t SaLWriteSampleToFlashMemory(struct sample sample) {
	
	

return (uint32_t)NULL;
}