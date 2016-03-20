/*
 * SaLFrostburn1.1.c
 *
 * Created: 3/9/2016 10:23:00 AM
 * Author : Stephen Arwine
 */


#include "sam.h"
#include <SaL.h>
#include <SaLUSART.h>
#include <SaLPort.h>
#include <SaLSample.h>
#include <SaLUsb.h>


int main(void) {
    /* Initialize the SAM system */
    SystemInit();

    SaLDelayInit();
    SalGclkInit();
    SaLRtcInit();
   // uart_init(9600);
    //SaLTC4Init();
    sampleInit();
    adcInit();

    pinOut(CC1120_SLAVE_SELECT);
    pinOut(AT25SF_SLAVE_SELECT);
    pinOut(MS5607_SLAVE_SELECT);
    pinOut(ADXL345_SLAVE_SELECT);
    // SaLPinMode(PIN_PA13,INPUT);

   // pinOut(MS5607_MOSI);
   // pinOut(MS5607_SCK);
   //pinIn(MS5607_MISO);




    pinHigh(CC1120_SLAVE_SELECT);
    pinHigh(AT25SF_SLAVE_SELECT);
    pinHigh(MS5607_SLAVE_SELECT);
    pinHigh(ADXL345_SLAVE_SELECT);



    //pinOut(LedPin);


    struct AccelerometerModule myAccelerometer;
    initAccelerometer(&myAccelerometer);
    initBarometer();
    //SaLFlashMemInit();

    /* Replace with your application code */

//     uint8_t message[255];
// // 	     AT25SFErace4KBlock(0);
// // 	     AT25SFWriteByte(0x00101,252);
// // 	   volatile uint8_t byte = AT25SFGetByte(0x00101);
// 
//     pinLow(CC1120_SLAVE_SELECT);
//     volatile uint8_t ccstatus = syncByte(CC1120_SCK ,CC1120_MOSI, CC1120_MISO, 0x80 | 0x30);
//     pinHigh(CC1120_SLAVE_SELECT);
// 
//     pinLow(CC1120_SLAVE_SELECT);
// 
//     while (pinRead(CC1120_MISO));
//     volatile uint8_t ccstatus2 = syncByte(CC1120_SCK ,CC1120_MOSI, CC1120_MISO, 0x80 | 0x3B);
//     volatile uint8_t ccstatus3 = getByte(CC1120_SCK_PIN,CC1120_MISO_PIN);
//     // byteOut(CC1120_SCK_PIN,CC1120_MOSI_PIN, 0b10111101 );
//     pinHigh(CC1120_SLAVE_SELECT);

    volatile float batt;

    while (1) {

        //delay_ms(10000);
        //pinToggle(LedPin);

        sampleTick();
        //batt = senseBatVolts(senseBat);
    }
}
