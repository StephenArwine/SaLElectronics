/*
 * SaLAltimeter-1.0.c
 *
 * Created: 1/17/2016 10:58:10 PM
 * Author : Stephen Arwine
 */

#include "sam.h"

#include <SaL.h>
#include <SaLUSART.h>
#include <SaLPort.h>
#include <SaLAltimeter.h>





volatile uint32_t time_ms = 0;
volatile uint32_t lasttimes[1000];

float accelDataX[1000];
float accelDataY[1000];
float accelDataZ[1000];
float currentHeight[1000];
uint8_t bytesRead;






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

void RTC_Handler(void) {
    time_ms += 1000;
    RTC->MODE1.INTFLAG.reg = 0xFF;
}

static uint32_t millis(void) {
    uint32_t ms;
    ATOMIC_SECTION_ENTER
    ms = time_ms + RTC->MODE1.COUNT.reg;
    if (RTC->MODE1.INTFLAG.bit.OVF)
        ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
    ATOMIC_SECTION_LEAVE
    return ms;
}

volatile uint32_t counter = 0;

int main(void) {
    SystemInit();
    SaLDelayInit();
    SalGclkInit();
    SaLRtcInit();
    PinConfig();
    uart_init(9600);
	
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
    uint32_t lasttime = 0;
    uint32_t index = 0;
    volatile uint32_t milliseconds = 0;

    uint8_t message[255];

    while (1) {

        counter++;
        index++;
        milliseconds = millis();

        if (milliseconds - lasttime > 10000) {
            lasttime = milliseconds;
            bytesRead = SaLIoRead(UsartIoModule,&message[0],225);
            MTK3329ParseMessage(&myGPS.MTK3329,&message[0]);
            SaLPlayTone(400);
            currentSample = getSample(myAltimeter);
        }

        getMS5607PressureSlow(&myBarometer);
        currentHeight[index] = myBarometer.currentAltInFt;

        getAccelEvent(myAltimeter.myAltimetersAccelerometer);
        accelDataX[index] = myAccelerometer.acceleration.Xf;
        accelDataY[index] = myAccelerometer.acceleration.Yf;
        accelDataZ[index] = myAccelerometer.acceleration.Zf;




        if (index == 1000) {
            index = 0;
            //SaLPlayTone(400);
        }
    }
}
