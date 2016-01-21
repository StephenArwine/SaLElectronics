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

    volatile uint16_t xreading = getBits(module,16);
    volatile uint16_t yreading = getBits(module,16);
    volatile uint16_t zreading = getBits(module,16);
    curaccelX = xreading;
    curaccelY = yreading;
    curaccelZ = zreading;
    SaLDigitalOut(module->SS,true);

}
int16_t currentX() {
    return (curaccelX);
}
int16_t currentY() {
    return (curaccelY);
}
int16_t currentZ() {
    return (curaccelZ);
}