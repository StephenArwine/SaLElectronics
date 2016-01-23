/*
 * SaLAltimeter-1.0.c
 *
 * Created: 1/17/2016 10:58:10 PM
 * Author : Stephen Arwine
 */

#include "sam.h"
#include <SaL.h>


float accelDataX[1000];
float accelDataY[1000];
float accelDataZ[1000];




void ClockInit() {

    SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
                           SYSCTRL_INTFLAG_DFLLRDY;

    NVMCTRL->CTRLB.bit.RWS = 1;

    // start and enable external 32k crystal
    SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE |
                           SYSCTRL_XOSC32K_XTALEN |
                           SYSCTRL_XOSC32K_EN32K |
                           ( 6 << SYSCTRL_XOSC32K_STARTUP_Pos);
    //wait for crystal to warm up
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_XOSC32KRDY)) == 0);
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(1) |
                       GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(1) |
                        GCLK_GENCTRL_SRC_XOSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(1) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID_DFLL48;

    //Configure the FDLL48MHz FLL, we will use this to provide a clock to the CPU
    //Set the course and fine step sizes, these should be less than 50% of the values used for the course and fine values (P150)
    SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_ENABLE); //Enable the DFLL
    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(7) | SYSCTRL_DFLLMUL_FSTEP(30));
    SYSCTRL->DFLLMUL.reg |= (SYSCTRL_DFLLMUL_MUL(1280));
    SYSCTRL->DFLLCTRL.reg |= (SYSCTRL_DFLLCTRL_MODE);

    //Wait and see if the DFLL output is good . . .
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);

    //For generic clock generator 0, select the DFLL48 Clock as input
    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(2)  | GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN));
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_CLKEN ) ;

    //set up OSC8M
}

void PinConfig() {

    /* temp SS HIGH for other peripherals */
    SaLPinMode(PIN_PA07,INPUT);
    SaLPinMode(PIN_PA10,OUTPUT);
    SaLPinMode(PIN_PA08,OUTPUT);
    SaLPinMode(BUZZER,OUTSTRONG);
    SaLDigitalOut(PIN_PA10,true);
    SaLDigitalOut(PIN_PA08,true);
}

 struct spiModule baroModuleSetup() {

    struct spiModule baroModule;
    configSpiModule(&baroModule,
                    MS5607_MOSI_PIN,
                    MS5607_MISO_PIN,
                    MS5607_SCK_PIN,
                    MS5607_SLAVE_SELECT_PIN);

    return baroModule;
}


struct USARTModule GPSmoduleSetup() {
    struct USARTModule gpsModule;
    configUSARTModule(&gpsModule,
                      PIN_PB23,
                      PIN_PB22,
                      5700,
                      5);

    return gpsModule;
}



void initBaroSensor(struct spiModule *const module ) {
    /*=========================================================================
    			baro init stuff
    -----------------------------------------------------------------------*/
    SaLDigitalOut(module->SS,false);
    byteOut(module,cmdReset_);
    SaLDigitalOut(module->SS,true);
    delay_us(30);
    read_coeff(module);
    /*=========================================================================*/
}

void initGPS() {



}

volatile uint32_t counter = 0;

int main(void) {
    SystemInit();
    ClockInit();
    SaLDelayInit();
    PinConfig();
	
	struct Accelerometer myAccelerometer;
	initAccelerometer(&myAccelerometer);
	
    struct spiModule baroModule =baroModuleSetup();
    struct USARTModule gpsModule =GPSmoduleSetup();

    initBaroSensor(&baroModule);
    initGPS(&gpsModule);

    volatile float accelX = 0;
    volatile float accelY = 0;
    volatile float accelZ = 0;

    SaLPlayTone(900);
    SaLPlayTone(800);
    SaLPlayTone(700);
    SaLPlayTone(600);
    SaLPlayTone(500);

    uint32_t index = 0;
    while (1) {
        counter++;
        getevents(myAccelerometer.thisSpiModule);
        accelX = currentX();
        accelY = currentY();
        accelZ = currentZ();

        accelDataX[index] = accelX;
        accelDataY[index] = accelY;
        accelDataZ[index] = accelZ;
        index++;
        if (index == 1000) {
            index = 0;
        }

    }

}
