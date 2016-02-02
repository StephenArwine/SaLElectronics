

#include <SaLBarometer.h>

uint32_t pressure;
uint32_t temperature;
uint32_t pascelFromPresTempConv;
uint32_t currentAltinCm;
uint32_t mills;
enum baroSampleState baroSampleState;

uint16_t hits;

bool baroSample(void) {

    switch (baroSampleState) {
    case baroSampleEmpty:
        sendMS5607D2ReadReq(cmdAdcD1_);
        baroSampleState++;
        mills = millis();
        return false;
        break;
    case baroSamplePressureRequested:
        //timer interrupt will send baroSampleState++
        if (millis() != mills) {
            baroSampleState++;
        } else {
            hits++;
        }
        return false;
        break;
    case baroSamplePressureReady:
        pressure = readMS5607AdcResults();
        baroSampleState++;
        return false;
        break;
    case baroSamplePressureRetreaved:
        sendMS5607D2ReadReq(cmdAdcD2_);
        baroSampleState++;
        return false;
        break;
    case baroSampleTemperatureRequested:
        if (millis() != mills) {
            baroSampleState++;
        } else {
            hits++;
        }
        return false;
        break;
    case baroSampleTemperatureReady:
        temperature = readMS5607AdcResults();
        baroSampleState++;
        return false;
        break;
    case baroSampleTemperatureRetreaved:
        pascelFromPresTempConv =ConvertPressureTemperature(&pressure, &temperature,&coefficients_[0]);
        baroSampleState++;
        return false;
        break;
    case baroSamplePascelCalculated:
        currentAltinCm = pascalToCent(pascelFromPresTempConv);
        baroSampleState++;
        return false;
        break;
    case baroSampleHeightCalculated:
        //some kind of baroSamplePut(currentAltinCm);
        return true;
        break;
    default:
        return false;
        break;
    };
};


void initBarometer(struct BarometerModule *const myBarometer) {

    baroSampleState = baroSampleEmpty;

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
    getMS5607Coeff(&coefficients_[0]);
#endif

}

void sendMS5607D2ReadReq(uint8_t _cmd) {

    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,FALSE);
    byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,_cmd);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,TRUE);
}

void getMS5607PressureSlow(struct BarometerModule *const myBarometer) {

    sendMS5607D2ReadReq(cmdAdcD1_);
    delay_us(700);
    myBarometer->pressure = readMS5607AdcResults();

    sendMS5607D2ReadReq(cmdAdcD2_);
    delay_us(700);
    myBarometer->temperature = readMS5607AdcResults();

    const uint32_t pressConv   = ConvertPressureTemperature(&myBarometer->pressure, &myBarometer->temperature,&coefficients_[0]);

    myBarometer->currentAltInFt = pascalToCent(pressConv);
    //myBarometer->currentAltInFt = paToFeetNOAA(pressConv);
}

uint32_t SaLBaroGetHeight() {
	return currentAltinCm;
};