

#include <SaLAccelerometer.h>

void initAccelerometer(
    struct Accelerometer *const myAccelerometer) {



#ifdef HAS_ADXL345

    struct spiModule tempAccelSpiModule;
    configSpiModule(&(myAccelerometer->thisSpiModule),
                    ADXL345_MOSI_PIN,
                    ADXL345_MISO_PIN,
                    ADXL345_SCK_PIN,
                    ADXL345_SLAVE_SELECT_PIN);

   // myAccelerometer->thisSpiModule = tempAccelSpiModule;

    myAccelerometer->rate_ = MY_ADXL345_DATA_RATE;
    myAccelerometer->range_ = MY_ADXL345_RANGE;

SaLDigitalOut(myAccelerometer->thisSpiModule->SS,FALSE);
byteOut(myAccelerometer->thisSpiModule,ADXL345_REG_POWER_CTL);
byteOut(myAccelerometer->thisSpiModule,0x00); // disable
SaLDigitalOut(myAccelerometer->thisSpiModule->SS,TRUE);

SaLDigitalOut(myAccelerometer->thisSpiModule->SS,FALSE);
byteOut(myAccelerometer->thisSpiModule,ADXL345_REG_BW_RATE);
byteOut(myAccelerometer->thisSpiModule,myAccelerometer->rate_);
SaLDigitalOut(myAccelerometer->thisSpiModule->SS,TRUE);

SaLDigitalOut(myAccelerometer->thisSpiModule->SS,FALSE);
byteOut(myAccelerometer->thisSpiModule,ADXL345_REG_BW_RATE);
byteOut(myAccelerometer->thisSpiModule,myAccelerometer->range_);
SaLDigitalOut(myAccelerometer->thisSpiModule->SS,TRUE);

SaLDigitalOut(myAccelerometer->thisSpiModule->SS,FALSE);
byteOut(myAccelerometer->thisSpiModule,ADXL345_REG_POWER_CTL);
byteOut(myAccelerometer->thisSpiModule,0x08); // disable
SaLDigitalOut(myAccelerometer->thisSpiModule->SS,TRUE);

#endif








}