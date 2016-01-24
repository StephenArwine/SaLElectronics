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
float currentHeight[1000];
volatile uint64_t time_ms = 0;

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

    //config xosc32k for the dfll via gen1
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(1) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(1) |
                        GCLK_GENCTRL_SRC_XOSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(1) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID_DFLL48;

    SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_ENABLE |
                          SYSCTRL_OSC32K_EN1K;
    //wait for crystal to warm up
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_OSC32KRDY)) == 0);
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(2) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                        GCLK_GENCTRL_SRC_OSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(2) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID(TC3_GCLK_ID);


    //Enable Peripheral Clock
    PM->APBCMASK.reg |= PM_APBCMASK_TC3;
    //Wait for regs to synchronize
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY));

    TC3->COUNT16.CTRLA.reg = TC_CTRLA_PRESCSYNC_PRESC |
                             TC_CTRLA_PRESCALER(TC_PRESCALE_256) |
                             TC_CTRLA_WAVEGEN_MFRQ |
                             TC_CTRLA_MODE_COUNT16;


    TC3->COUNT16.INTENSET.reg = TC_INTENSET_MC(MC0_INT_FLAG);
    NVIC_EnableIRQ(TC3_IRQn);
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY));

    TC3->COUNT16.CC[0].reg = 1000;
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY));

    TC3->COUNT16.COUNT.reg = 0x0000;
    while((TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY));

	TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;



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
    SaLPinMode(MS5607_SLAVE_SELECT_PIN,OUTPUT);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,TRUE);
}



volatile uint32_t counter = 0;

int main(void) {
    SystemInit();
    SaLDelayInit();
    ClockInit();
    PinConfig();

    Accelerometer myAccelerometer;
    initAccelerometer(&myAccelerometer);

    Barometer myBarometer;
    initBarometer(&myBarometer);

    getAccelEvent(&myAccelerometer);

    volatile float accelX = 0;
    volatile float accelY = 0;
    volatile float accelZ = 0;

    startUpTone();
    NVIC_EnableIRQ(RTC_IRQn);

    uint32_t index = 0;
    while (1) {
        counter++;

//         if (time_ms%1000 == 0) {
//             SaLPlayTone(500);
//         }

        getMS5607PressureSlow(&myBarometer);
        currentHeight[index] = myBarometer.currentAltInFt;

        getAccelEvent(&myAccelerometer);
        accelX = myAccelerometer.acceleration.Xf;
        accelY = myAccelerometer.acceleration.Yf;
        accelZ =myAccelerometer.acceleration.Zf;

        accelDataX[index] = accelX;
        accelDataY[index] = accelY;
        accelDataZ[index] = accelZ;
        index++;
        if (index == 1000) {
            index = 0;
        }
    }
}
