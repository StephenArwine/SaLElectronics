#include <ADXL345.h>

void setRange(struct spiModule *const module,
              ADXL345Range _cmd) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATA_FORMAT);
    byteOut(module,_cmd);
    SaLDigitalOut(module->SS,true);
}

void getevents(struct spiModule *const module) {
    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAX0 | 0x80 | 0x40);
    volatile uint8_t _byte1 = getByte(module);
    volatile uint8_t _byte2 = getByte(module);
    volatile uint8_t _byte3 = getByte(module);
    volatile uint8_t _byte4 = getByte(module);
    volatile uint8_t _byte5 = getByte(module);
    volatile uint8_t _byte6 = getByte(module);


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
        Yaccel = (_byte4 * 256) + _byte3;
    }

    negative = (_byte6 & (1 <<7)) != 0;
    if (negative) {
        Zaccel = ((_byte6 | ~((1 << 8) - 1)) << 8 ) | _byte5;
    } else {
        Zaccel = (_byte6 * 256) + _byte5;
    }

    SaLDigitalOut(module->SS,true);
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