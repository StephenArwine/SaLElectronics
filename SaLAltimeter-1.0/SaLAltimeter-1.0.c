/*
 * SaLAltimeter-1.0.c
 *
 * Created: 1/17/2016 10:58:10 PM
 * Author : Stephen Arwine
 */

//#include "sam.h"

#include <SaL.h>
#include <SaLUSART.h>
#include <SaLPort.h>
#include <SaLSample.h>

volatile uint32_t lasttimes[1000];

float accelDataX[1000];
float accelDataY[1000];
float accelDataZ[1000];
int32_t currentHeight[1000];
uint8_t bytesRead;
bool retrieveSample;
uint32_t index2;


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

void TC5_Handler(void) {
    //SaLPlayTone(400);
    ATOMIC_SECTION_ENTER
    index2++;
    TC5->COUNT8.INTFLAG.reg = 0XFF;
    ATOMIC_SECTION_LEAVE
}

void TC4_Handler(void) {
    retrieveSample = true;
    TC4->COUNT16.INTFLAG.reg = 0XFF;
}

struct sample cookedSample;

volatile uint32_t counter = 0;

struct sample cookedSample;

int main(void) {
    // SystemInit();
    SaLDelayInit();
    SalGclkInit();
    SaLRtcInit();
    PinConfig();
    uart_init(9600);
    SaLTC4Init();
    // SaLTC5Init();

    struct IoDescriptor *UsartIoModule;
    struct GpsModule myGPS;
    struct MTK3329Module MTK3329Instance;
    struct AccelerometerModule myAccelerometer;
    struct BarometerModule myBarometer;
    struct AltimeterModule myAltimeter;

    SaLSyncUsartIo(&USART_0, &UsartIoModule);
    initAccelerometer(&myAccelerometer);
    initBarometer(&myBarometer);
    getAccelEvent(&myAccelerometer);

    myGPS.MTK3329 = MTK3329Instance;
    myAltimeter.myAltimetersAccelerometer = &myAccelerometer;
    myAltimeter.myAltimetersBarometer = &myBarometer;
    myAltimeter.myAltimetersGps = &myGPS;

    //startUpTone();

    //uint32_t index = 0;
    volatile uint32_t milliseconds = 0;

    uint8_t message[255];

    struct sVar groundHeight;

    for (uint8_t i = 0; i < 100; i++) {
        getMS5607PressureSlow(&myBarometer);
        uint32_t tempheight = myBarometer.currentAltInFt;
        addSampleToVariance(&groundHeight,tempheight);
    }
    volatile uint32_t variance = GetVariance(&groundHeight,&groundHeight.mean);


    for (uint8_t i = 0; i < 200; i++) {
        getMS5607PressureSlow(&myBarometer);
        uint32_t tempheight = myBarometer.currentAltInFt;
        addSampleToVariance(&groundHeight,tempheight);

    }

    volatile int32_t groundAlt = groundHeight.mean;
    variance = GetVariance(&groundHeight,&groundHeight.mean);

    volatile uint16_t ticks = 0;

    while (1) {
        ticks++;
        counter++;
        // milliseconds = millis();

        sampleTick();
        if (ticks > 1000) {
            ticks = 0;
        }
    }
}
