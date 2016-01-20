/*
 * SaLAltimeter-1.0.c
 *
 * Created: 1/17/2016 10:58:10 PM
 * Author : Stephen Arwine
 */


#define HAS_MS5607 1
#define MS5607_MOSIPIN PIN_PA13
#define MS5607_MISOPIN PIN_PA14
#define MS5607_SCKPIN PIN_PA15
#define MS5607_SLAVE_SELECT_PIN PIN_PA09

#define HAS_ADXL345 1
#define ADXL345_MOSIPIN PIN_PA13
#define ADXL345_MISOPIN PIN_PA14
#define ADXL345_SCKPIN PIN_PA15
#define ADXL345_SLAVE_SELECT_PIN PIN_PA16

#define NVM_DFLL_COARSE_POS    58
#define NVM_DFLL_COARSE_SIZE   6
#define NVM_DFLL_FINE_POS      64
#define NVM_DFLL_FINE_SIZE     10

#define BUZZER PIN_PA12

#include "sam.h"
#include <samd21g18a.h>
#include <SaL.h>




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
                    MS5607_MOSIPIN,
                    MS5607_MISOPIN,
                    MS5607_SCKPIN,
                    MS5607_SLAVE_SELECT_PIN);

    return baroModule;
}

struct spiModule accelModuleSetup() {
    struct spiModule accelModule;
    configSpiModule(&accelModule,
                    ADXL345_MOSIPIN,
                    ADXL345_MISOPIN,
                    ADXL345_SCKPIN,
                    ADXL345_SLAVE_SELECT_PIN);

    return accelModule;
}

void initAccelSensor(struct spiModule *const module) {

    /*=========================================================================
    					accel init stuff
    -----------------------------------------------------------------------*/
    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_POWER_CTL);
    byteOut(module,0x00);
    SaLDigitalOut(module->SS,true);

    setRange(module,ADXL345_RANGE_2_G);
//     SaLDigitalOut(accelModule.SS,false);
//     byteOut(&accelModule,ADXL345_REG_DATA_FORMAT);
//     byteOut(&accelModule,0b10000000);
//     SaLDigitalOut(accelModule.SS,true);


    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_BW_RATE);
    byteOut(module,0b1110);
    SaLDigitalOut(module->SS,true);


    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_POWER_CTL);
    byteOut(module,0x08);
    SaLDigitalOut(module->SS,true);
    /*=========================================================================*/


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

volatile uint32_t counter = 0;

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    ClockInit();
    SaLDelayInit();
    PinConfig();
    struct spiModule baroModule =baroModuleSetup();
    struct spiModule accelModule =accelModuleSetup();

    initAccelSensor(&accelModule);
    initBaroSensor(&baroModule);



    volatile int16_t accelX = getX(&accelModule);
    volatile int16_t accelY =  getY(&accelModule);
    volatile int16_t accelZ =  getZ(&accelModule);

    //delay_ms(1000);
    SaLPlayTone(100);
    SaLPlayTone(200);
    SaLPlayTone(300);
    SaLPlayTone(400);
    SaLPlayTone(500);
    SaLPlayTone(800);
    SaLPlayTone(900);
	delay_ms(10000);
    SaLPlayTone(170);






    while (1) {
        counter++;
        getevents(&accelModule);
        accelX = currentX();
        accelY = currentY();
        accelZ = currentZ();

    }
}
