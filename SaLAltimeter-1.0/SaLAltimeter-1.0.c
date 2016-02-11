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
#include <SaLUsb.h>



uint32_t bytesRead;
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

void TC4_Handler(void) {
    retrieveSample = true;
    TC4->COUNT16.INTFLAG.reg = 0XFF;
}

struct sample cookedSample;

volatile uint32_t counter = 0;

void usbInit2() {



}

int main(void) {
    SystemInit();
    SaLDelayInit();
    SalGclkInit();
    SaLRtcInit();
    PinConfig();
    uart_init(9600);
    SaLTC4Init();
    sampleInit();
    // UsbInit();


    struct IoDescriptor *UsartIoModule;
    struct IoDescriptor *UsartIoModuleUsb;

    struct AccelerometerModule myAccelerometer;

    SaLSyncUsartIo(&USART_0, &UsartIoModule);
    SaLSyncUsartIo(&USART_1, &UsartIoModuleUsb);

    initAccelerometer(&myAccelerometer);
    initBarometer();
	SaLFlashMemInit();

    volatile uint16_t ticks = 0;
    uint32_t milliseconds = 0;
    uint32_t lastTime = 0;
    uint8_t message[255];
//     AT25SFErace4KBlock(0);
//     AT25SFWriteByte(0x00101,251);
//   volatile uint8_t byte = AT25SFGetByte(0x00101);

    while (1) {
        ticks++;
        counter++;
        milliseconds = millis();
        if (milliseconds - lastTime > 150000*3.3) {
            // bytesRead = SaLIoRead(UsartIoModule,&message[0],255);
            lastTime = milliseconds;
            //SaLPlayTone(400);
        }
        sampleTick();

    }


}
