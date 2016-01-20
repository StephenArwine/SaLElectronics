#include <ADXL345.h>

void setRange(struct spiModule *const module,
              ADXL345Range _cmd) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATA_FORMAT);
    byteOut(module,_cmd);
    SaLDigitalOut(module->SS,true);
}

int16_t getX(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAX0 | 0x80);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);

    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;

    return _receive;
}

int16_t getY(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAY0|0x80);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);

    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;

    return _receive;
}

int16_t getZ(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAZ0|0x80);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);

    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;

    return _receive;
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
    SaLDigitalOut(module->SS,true);

    curaccelX = _byte1;
    curaccelX = (curaccelX * 256) + _byte2;
    curaccelY = _byte3;
    curaccelY = (curaccelY * 256) + _byte4;
    curaccelZ = _byte5;
    curaccelZ = (curaccelZ * 256) + _byte6;
}

int16_t currentX() {
    return (curaccelX * ADXL345_MG2G_MULTIPLIER);
}
int16_t currentY() {
    return (curaccelY * ADXL345_MG2G_MULTIPLIER);
}
int16_t currentZ() {
    return (curaccelZ * ADXL345_MG2G_MULTIPLIER);
}