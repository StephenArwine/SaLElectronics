

#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3
#define TRUE 1
#define FALSE 0

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <boardDefines.h>


#include <SaLPort.h>



#include <SaLUSART.h>

#include <SaLBitBang.h>

#include <SaLSpi.h>

#include <SaLDelay.h>

#include <SaLBuzzer.h>


#ifdef HAS_MS5607
#include <MS5607.h>
#endif

#ifdef HAS_ADXL345
#include <ADXL345.h>
#endif

#include <SaLAccelerometer.h>
#include <SaLBarometer.h>

#include <SaLAltimeter.h>


