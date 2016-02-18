
#ifndef SAL_H_
#define SAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <compiler.h>


#define INPUT 0
#define OUTPUT 1
#define OUTSTRONG 3
#define TRUE 1
#define FALSE 0


#define CONTAINER_OF(ptr, type, field_name) \
((type *) (((uint8_t *)ptr) - offsetof(type, field_name)))

#include <SaLMath.h>


#include <boardDefines.h>

#include <samd21g18a.h>
#include <samd21.h>

#include <SaLClocks.h>

#include <SaLPort.h>

#include <SaLAnalog.h>

#include <SaLBitBang.h>

#include <SaLDelay.h>

#include <SaLSenseBat.h>

#include <SaLBuzzer.h>

//#include <SaLAltimeter.h>


#ifdef HAS_MS5607
#include <MS5607.h>
#endif

#ifdef HAS_ADXL345
#include <ADXL345.h>
#endif

#ifdef HAS_MTK3329
#include <MTK3329.h>
#endif

#ifdef HAS_AT25SF
#include <AT25SF.h>
#endif

#ifdef HAS_FLASHMEM
#include <SaLFlashMem.h>
#endif


#endif

