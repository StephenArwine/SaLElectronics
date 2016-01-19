#ifndef SAL_H_
#define SAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <samd21g18a.h>

#include <SaLPort.h>

#include <SaLBitBang.h>

#include <SaLDelay.h>

#ifdef HAS_MS5607
#include <MS5607.h>
#endif

#ifdef HAS_ADXL345
#include <ADXL345.h>
#endif






#endif