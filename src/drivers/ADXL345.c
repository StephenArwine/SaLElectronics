#include <ADXL345.h>
#include <SaLPort.h>
#include <SaLBitBang.h>

void setRange(
    ADXL345Range _cmd) {

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_DATA_FORMAT);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,_cmd);
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);
}

void getADXL345Event(uint8_t *values) {

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_DATAX0 | 0x80 | 0x40);
    for (uint8_t i = 0; i < 6; i++) {
        *(values+i) = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
    }
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);
}

void getevents() {
    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,false);
    byteOut(ADXL345_SCK_PIN,ADXL345_MOSI_PIN,ADXL345_REG_DATAX0 | 0x80 | 0x40);
     uint8_t _byte1 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
     uint8_t _byte2 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
     uint8_t _byte3 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
     uint8_t _byte4 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
     uint8_t _byte5 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);
     uint8_t _byte6 = getByte(ADXL345_SCK_PIN,ADXL345_MISO_PIN);

    SaLDigitalOut(ADXL345_SLAVE_SELECT_PIN,true);



    //check if last bit in second byte is 1 therefor negative number
    int16_t negative = (_byte2 & (1 <<7)) != 0;
    if (negative) {
        // if negative then preform 2's complement to int conversion
        Xaccel = ((_byte2 | ~((1 << 8) - 1)) << 8 ) | _byte1;
    } else {
        Xaccel = (_byte2 << 8) + _byte1;
    }
    negative = (_byte4 & (1 <<7)) != 0;
    if (negative) {
        Yaccel = ((_byte4 | ~((1 << 8) - 1)) << 8 ) | _byte4;
    } else {
        Yaccel = (_byte4 << 8) | _byte3;
    }
    negative = (_byte6 & (1 <<7)) != 0;
    if (negative) {
        Zaccel = ((_byte6 | ~((1 << 8) - 1)) << 8 ) | _byte5;
    } else {
        Zaccel = (_byte6 << 8) | _byte5;
    }
}

float currentX() {
    return (Xaccel * ADXL345_MG2G_MULTIPLIER);
}
float currentY() {
    return (Yaccel * ADXL345_MG2G_MULTIPLIER);
}
float currentZ() {
    return (Zaccel * ADXL345_MG2G_MULTIPLIER);
}