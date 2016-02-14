

#include <SaLAccelerometer.h>
#include <SaLPort.h>
#include <SaLBitBang.h>

enum accelSampleState accelSampleState;
uint8_t regAccelData[6];
int16_t X;
int16_t Y;
int16_t Z;
float Xf;
float Yf;
float Zf;

bool accelSample() {

    switch(accelSampleState) {
    case accelSampleEmpty:
        getADXL345Event(&regAccelData[0]);
        accelSampleState++;
        return false;
        break;
    case accelSampleretreaved:
        X = Convert2C(&regAccelData[0]);
        accelSampleState++;
        return false;
        break;
    case accelSampleXIntConverted:
        Y = Convert2C(&regAccelData[2]);
        accelSampleState++;
        return false;
        break;
    case accelSampleYIntConverted:
        Z = Convert2C(&regAccelData[4]);
        accelSampleState++;
        return false;
        break;
    case accelSampleZIntConverted:
        Xf = X * ADXL345_MG2G_MULTIPLIER;
        accelSampleState++;
        return false;
        break;
    case accelSampleXFloatConverted:
        Yf = Y * ADXL345_MG2G_MULTIPLIER;
        accelSampleState++;
        return false;
        break;
    case accelSampleYFloatConverted:
        Zf = Z * ADXL345_MG2G_MULTIPLIER;
        accelSampleState++;
        return false;
        break;
    case accelSapleCooked:

        return true;
        break;
		
    default:
        return false;
        break;

    }

}





void initAccelerometer(struct AccelerometerModule *myAccelerometer) {

    accelSampleState = accelSampleEmpty;



#ifdef HAS_ADXL345

    SaLPinMode(ADXL345_SLAVE_SELECT_PIN,OUTPUT);
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);
    SaLPinMode(ADXL345_MOSI_PIN,OUTPUT);
    SaLPinMode(ADXL345_SCK_PIN,OUTPUT);
    SaLPinMode(ADXL345_MISO_PIN,INPUT);

    myAccelerometer->rate_ = MY_ADXL345_DATA_RATE;
    myAccelerometer->range_ = MY_ADXL345_RANGE;

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_POWER_CTL);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,0x00); // disable
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_BW_RATE);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,myAccelerometer->rate_);
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_BW_RATE);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,myAccelerometer->range_);
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_POWER_CTL);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,0x08); // enable
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);

#endif

}


float SaLGetAccelX(){
	return Xf;
}
float SaLGetAccelY(){
	return Yf;
}
float SaLGetAccelZ(){
	return Zf;
}