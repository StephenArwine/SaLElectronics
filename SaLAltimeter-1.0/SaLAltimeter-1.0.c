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
extern uint32_t milliseconds = 0;



void PinConfig() {
    /* temp SS HIGH for other peripherals */
    SaLPinMode(PIN_PA07,INPUT);
    SaLPinMode(PIN_PA10,OUTPUT);
    SaLPinMode(PIN_PA08,OUTPUT);
    //SaLPinMode(BUZZERPIN,OUTSTRONG);
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
    //SaLTC4Init();
    sampleInit();
    adcInit();
    // UsbInit();
    // startUpTone();
    SaLBuzzerInit();



    struct IoDescriptor *UsartIoModule;
    struct IoDescriptor *UsartIoModuleUsb;

    struct AccelerometerModule myAccelerometer;

    SaLSyncUsartIo(&USART_0, &UsartIoModule);
    SaLSyncUsartIo(&USART_1, &UsartIoModuleUsb);

    initAccelerometer(&myAccelerometer);
    initBarometer();
    SaLFlashMemInit();


    volatile uint16_t ticks = 0;

    uint32_t lastTime = 0;
    //uint8_t message[255];
//     AT25SFErace4KBlock(0);
//     AT25SFWriteByte(0x00101,251);
//   volatile uint8_t byte = AT25SFGetByte(0x00101);

//     pinLow(CC1120_SLAVE_SELECT);
//     delay_ms(30);
//     pinHigh(CC1120_SLAVE_SELECT);
//     delay_ms(20);
//     pinLow(CC1120_SLAVE_SELECT);
//     delay_ms(30);
//     pinHigh(CC1120_SLAVE_SELECT);
//     delay_ms(20);

    pinLow(CC1120_SLAVE_SELECT);
    volatile uint8_t ccstatus = syncByte(CC1120_SCK ,CC1120_MOSI, CC1120_MISO, 0x80 | 0x30);
    pinHigh(CC1120_SLAVE_SELECT);

    //delay_ms(200);

    pinLow(CC1120_SLAVE_SELECT);

    // while (pinRead(CC1120_MISO));
    volatile uint8_t ccstatus2 = syncByte(CC1120_SCK ,CC1120_MOSI, CC1120_MISO, 0x80 | 0x3B);
    volatile uint8_t ccstatus3 = getByte(CC1120_SCK_PIN,CC1120_MISO_PIN);
    // byteOut(CC1120_SCK_PIN,CC1120_MOSI_PIN, 0b10111101 );
    pinHigh(CC1120_SLAVE_SELECT);

     startUpTone();
    startUpTone();
	
    volatile float batt;

    //TC5->COUNT16.CTRLA.bit.ENABLE = 0;

    TC5->COUNT16.CTRLBCLR.reg= TC_CTRLBCLR_CMD_RETRIGGER;

    while (1) {

        milliseconds = millis();

        sampleTick(); 
        batt = senseBatVolts(senseBat);
    }
}
