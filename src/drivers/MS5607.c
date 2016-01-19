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
	volatile uint8_t _byte1 = get_byte(module);
	volatile uint8_t _byte2 = get_byte(module);
	volatile uint8_t _byte3 = get_byte(module);
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

void convertPaToFt(void){

	//struct MS5607_module module = baroModule;

	//module->heightFt = paToFeetNOAA(module->rawPressureData);


}

uint32_t paToFeetNOAA(const int32_t pressurePa) {
	double pressMillibar = 0.01 *pressurePa;

	double altFeet = (1-pow((pressMillibar/1013.25),(0.190284)))*145366.45;
	return (uint32_t)altFeet;
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