#include <MS5607.h>
#include <math.h>



uint32_t read_Adc(struct spiModule *const module,
                  uint8_t _cmd) {
    SaLDigitalOut(module->SS,false);
    byteOut(module,_cmd);
    SaLDigitalOut(module->SS,true);
    delay_us(900);

    SaLDigitalOut(module->SS,false);
    byteOut(module,cmdAdcRead_);
    volatile uint8_t _byte1 = getByte(module);
    volatile uint8_t _byte2 = getByte(module);
    volatile uint8_t _byte3 = getByte(module);
    SaLDigitalOut(module->SS,true);

    uint32_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;
    _receive = (_receive * 256) + _byte3;

    return _receive;

}

void read_coeff(struct spiModule *const module) {

    uint16_t coefficient;
    for (uint8_t coeff_num = 0; coeff_num < 6 ; ++coeff_num ) {

        uint8_t _cmd = cmdPromRd_ + ((coeff_num+1)*2);
        delay_us(300);
        SaLDigitalOut(module->SS,false);
        byteOut(module,_cmd);
        uint8_t _byte1 = getByte(module);
        uint8_t _byte2 = getByte(module);

        SaLDigitalOut(module->SS,true);

        coefficient = _byte1;
        coefficient = (coefficient *256 )+ _byte2;
        coefficients_[coeff_num] = coefficient;
    }
}

uint32_t ConvertPressureTemperature(uint32_t pressure, uint32_t temperature) {
    const uint64_t C1 = (uint64_t)(coefficients_[0]);
    const uint64_t C2 = (uint64_t)(coefficients_[1]);
    const uint64_t C3 = (uint64_t)(coefficients_[2]);
    const uint64_t C4 = (uint64_t)(coefficients_[3]);
    const uint64_t C5 = (uint64_t)(coefficients_[4]);
    const uint64_t C6 = (uint64_t)(coefficients_[5]);

    // calculate 1st order pressure and temperature (MS5607 1st order algorithm)
    const volatile int32_t dT    = temperature - (C5 * 256);
    const volatile int32_t temp  = 2000 + (dT * C6) / (8388608) ;

    const volatile int64_t OFF   = (C2 * (131072)) + ((C4 * dT) / (64));
    const volatile int64_t SENS  = (C1 * (65536)) + ((C3 * dT) / (128));
    const volatile int32_t press = ((pressure * SENS / (2097152) - OFF) / (32768));
    return press;
}

uint32_t getAltFt(struct spiModule *const module) {

    const uint32_t temperature = read_Adc(module, cmdAdcD2_);
    const uint32_t pressure    = read_Adc(module, cmdAdcD1_);
    const uint32_t pressConv   = ConvertPressureTemperature(pressure, temperature);

    //const int32_t AltCm = pascalToCent(pressConv);
    volatile uint32_t AltFt = paToFeetNOAA(pressConv);

    return AltFt;
}


uint32_t paToFeetNOAA(const int32_t pressurePa) {
    double pressMillibar = 0.01 *pressurePa;

    double altFeet = (1-pow((pressMillibar/1013.25),(0.190284)))*145366.45;
    return (uint32_t)altFeet;
}

