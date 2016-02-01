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

void TC3_Handler(void) {
    retrieveSample = true;
    TC3->COUNT8.INTFLAG.reg = 0XFF;
}

volatile uint32_t counter = 0;

int main(void) {
    SystemInit();
    SaLDelayInit();
    SalGclkInit();
    SaLRtcInit();
    PinConfig();
    uart_init(9600);
    SaLTC3Init();

    struct IoDescriptor *UsartIoModule;
    struct GpsModule myGPS;
    struct MTK3329Module MTK3329Instance;
    struct AccelerometerModule myAccelerometer;
    struct BarometerModule myBarometer;
    struct AltimeterModule myAltimeter;
    struct sample currentSample;

    SaLSyncUsartIo(&USART_0, &UsartIoModule);
    initAccelerometer(&myAccelerometer);
    initBarometer(&myBarometer);
    getAccelEvent(&myAccelerometer);

    myGPS.MTK3329 = MTK3329Instance;
    myAltimeter.myAltimetersAccelerometer = &myAccelerometer;
    myAltimeter.myAltimetersBarometer = &myBarometer;
    myAltimeter.myAltimetersGps = &myGPS;

    //startUpTone();
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_EnableIRQ(TC3_IRQn);
    uint32_t index = 0;
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

    uint32_t lasttime = millis();

    while (1) {

        counter++;
        milliseconds = millis();

        if (milliseconds - lasttime > 10000) {
            lasttime = milliseconds;
            bytesRead = SaLIoRead(UsartIoModule,&message[0],225);
            MTK3329ParseMessage(&myGPS.MTK3329,&message[0]);
            // SaLPlayTone(400);
            currentSample = getSample(myAltimeter);
            //counter = 0;
        }

        if (retrieveSample) {

            getMS5607PressureSlow(&myBarometer);
            currentHeight[index] = groundAlt - myBarometer.currentAltInFt;

            getAccelEvent(myAltimeter.myAltimetersAccelerometer);
            accelDataX[index] = myAccelerometer.acceleration.Xf;
            accelDataY[index] = myAccelerometer.acceleration.Yf;
            accelDataZ[index] = myAccelerometer.acceleration.Zf;
            retrieveSample = false;
            index++;
        }
        if (index == 500) {
            index = 0;
            SaLPlayTone(400);
        }
    }
}
