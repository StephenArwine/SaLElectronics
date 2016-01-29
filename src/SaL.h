#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3
#define TRUE 1
#define FALSE 0

#define CONTAINER_OF(ptr, type, field_name) \
((type *) (((uint8_t *)ptr) - offsetof(type, field_name)))


#include <boardDefines.h>

#include <samd21g18a.h>
#include <samd21.h>

#include <SaLClocks.h>

#include <SaLPort.h>

#include <SaLBitBang.h>

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


