

#include <SaLBarometer.h>



void initBarometer(Barometer *const myBarometer) {

#ifdef HAS_MS5607
    SaLPinMode(MS5607_SLAVE_SELECT_PIN,OUTPUT);
    SaLPinMode(MS5607_SLAVE_SELECT_PIN,TRUE);
    SaLPinMode(MS5607_MOSI_PIN,OUTPUT);
    SaLPinMode(MS5607_SCK_PIN,OUTPUT);
    SaLPinMode(MS5607_MISO_PIN,INPUT);

    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,FALSE);
    byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,MS5607_CMD_RES);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,TRUE);
    delay_us(600);
    getMS5607Coeff(&myBarometer->coefficients_[0]);
#endif

}

void sendMS5607D2ReadReq(uint8_t _cmd) {

    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,FALSE);
    byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,_cmd);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,TRUE);
}

void getMS5607PressureSlow(Barometer *const myBarometer) {

    sendMS5607D2ReadReq(cmdAdcD1_);
    delay_us(900);
    myBarometer->pressure = readMS5607AdcResults();

    sendMS5607D2ReadReq(cmdAdcD2_);
    delay_us(900);
    myBarometer->temperature = readMS5607AdcResults();

    const uint32_t pressConv   = ConvertPressureTemperature(&myBarometer->pressure, &myBarometer->temperature,&myBarometer->coefficients_[0]);

    myBarometer->currentAltInFt = paToFeetNOAA(pressConv);

}