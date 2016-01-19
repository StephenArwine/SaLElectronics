#include <ADXL345.h>

void setRange(struct spiModule *const module,
              ADXL345Range _cmd) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,0x31);
    byteOut(module,_cmd);
    SaLDigitalOut(module->SS,true);
}

int16_t getX(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAX0);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);
    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;
    return _receive;
}

int16_t getY(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAY0);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);
    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;
    return _receive;
}

int16_t getZ(struct spiModule *const module) {

    SaLDigitalOut(module->SS,false);
    byteOut(module,ADXL345_REG_DATAZ0);
    uint8_t _byte1 = getByte(module);
    uint8_t _byte2 = getByte(module);
    SaLDigitalOut(module->SS,true);
    uint16_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;
    return _receive;
}