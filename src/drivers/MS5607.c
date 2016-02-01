#include <MS5607.h>
#include <math.h>



uint32_t read_Adc(uint8_t _cmd) {
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,false);
    byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,_cmd);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,true);
    delay_us(900);

    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,false);
    byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,cmdAdcRead_);
    volatile uint8_t _byte1 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
    volatile uint8_t _byte2 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
    volatile uint8_t _byte3 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
    SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,true);

    uint32_t _receive = _byte1;
    _receive = (_receive * 256) + _byte2;
    _receive = (_receive * 256) + _byte3;

    return _receive;

}

uint32_t readMS5607AdcResults(){
	
	 SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,false);
	 byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,cmdAdcRead_);
	 volatile uint8_t _byte1 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
	 volatile uint8_t _byte2 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
	 volatile uint8_t _byte3 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
	 SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,true);
	 
	 uint32_t _receive = (_byte1 << 16) | (_byte2 << 8) | (_byte3);
	 return _receive;
}

void getMS5607Coeff(uint16_t *coefficients) {

    for (uint8_t coeff_num = 0; coeff_num < 6 ; ++coeff_num ) {
        delay_us(600);
        uint8_t _cmd = MS5607_CMD_PROM_READ + ((coeff_num+1)*2);
        delay_us(600);
        SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,false);
        byteOut(MS5607_SCK_PIN,MS5607_MOSI_PIN,_cmd);
        uint8_t _byte1 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
        uint8_t _byte2 = getByte(MS5607_SCK_PIN,MS5607_MISO_PIN);
        SaLDigitalOut(MS5607_SLAVE_SELECT_PIN,true);
        *(coefficients+coeff_num) = (_byte1 << 8) | _byte2;
    }
}
uint32_t ConvertPressureTemperature(uint32_t *pressure, uint32_t *temperature, uint16_t *coefficients) {
    const uint64_t C1 = (uint64_t)(*coefficients);
    const uint64_t C2 = (uint64_t)(*(coefficients + 1));
    const uint64_t C3 = (uint64_t)(*(coefficients + 2));
    const uint64_t C4 = (uint64_t)(*(coefficients + 3));
    const uint64_t C5 = (uint64_t)(*(coefficients + 4));
    const uint64_t C6 = (uint64_t)(*(coefficients + 5));

    // calculate 1st order pressure and temperature (MS5607 1st order algorithm)
    const volatile int32_t dT    = *temperature - (C5 * 256);
    const volatile int32_t temp  = 2000 + (dT * C6) / (8388608) ;

    const volatile int64_t OFF   = (C2 * (131072)) + ((C4 * dT) / (64));
    const volatile int64_t SENS  = (C1 * (65536)) + ((C3 * dT) / (128));
    const volatile int32_t press = ((*pressure * SENS / (2097152) - OFF) / (32768));
    return press;
}


float paToFeetNOAA(const int32_t pressurePa) {
    double pressMillibar = 0.01 *pressurePa;

    double altFeet = (1-pow((pressMillibar/1013.25),(0.190284)))*145366.45;
    return (float)altFeet;
}

uint32_t pascalToCent(const int32_t pressurePa) {

    static const int32_t PZLUT_ENTRIES = 77;
    static const int32_t PA_INIT       = 104908;
    static const int32_t PA_DELTA      = 1024;

    static const int32_t lookupTable[] = {
        -29408, -21087, -12700,  -4244,   4279,
        12874,  21541,  30281,  39095,  47986,
        56953,  66000,  75126,  84335,  93628,
        103006, 112472, 122026, 131672, 141410,
        151244, 161174, 171204, 181335, 191570,
        201911, 212361, 222922, 233597, 244388,
        255300, 266334, 277494, 288782, 300204,
        311761, 323457, 335297, 347285, 359424,
        371719, 384174, 396795, 409586, 422552,
        435700, 449033, 462560, 476285, 490216,
        504360, 518724, 533316, 548144, 563216,
        578543, 594134, 609999, 626149, 642595,
        659352, 676431, 693847, 711615, 729752,
        748275, 767202, 786555, 806356, 826627,
        847395, 868688, 890537, 912974, 936037,
        959766, 984206
    };

    if (pressurePa > PA_INIT)
        return lookupTable[0];
    else {
        const int32_t inx = (PA_INIT - pressurePa) >> 10;
        if (inx >= PZLUT_ENTRIES - 1)
            return lookupTable[PZLUT_ENTRIES - 1];
        else {
            const int32_t pa1 = PA_INIT - (inx << 10);
            const int32_t z1 = lookupTable[inx];
            const int32_t z2 = lookupTable[inx + 1];
            return (z1 + (((pa1 - pressurePa) * (z2 - z1)) >> 10));
        }
    }
}