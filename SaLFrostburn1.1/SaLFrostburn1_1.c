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
    uart_init(9600);
    //SaLTC4Init();
    sampleInit();
    adcInit();

    pinOut(CC1120_SLAVE_SELECT);
    pinOut(AT25SF_SLAVE_SELECT);
    pinOut(MS5607_SLAVE_SELECT);
    pinOut(ADXL345_SLAVE_SELECT);
    SaLPinMode(PIN_PA13,INPUT);

    pinOut(MS5607_MOSI);
    pinOut(MS5607_SCK);
    pinIn(MS5607_MISO);




    pinHigh(CC1120_SLAVE_SELECT);
    pinHigh(AT25SF_SLAVE_SELECT);
    pinHigh(MS5607_SLAVE_SELECT);
    pinHigh(ADXL345_SLAVE_SELECT);






    struct AccelerometerModule myAccelerometer;
    initAccelerometer(&myAccelerometer);
    initBarometer();
    SaLFlashMemInit();

    /* Replace with your application code */

    volatile float batt;

    while (1) {

        sampleTick();
        batt = senseBatVolts(senseBat);
    }
}
