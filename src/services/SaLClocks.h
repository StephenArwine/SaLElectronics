
#ifndef SALCLOCK_H_
#define SALCLOCK_H_

#include <SaLTypes.h>
#include <boardDefines.h>

void SalGclkInit();
void SaLRtcInit();

void RTC_Handler(void);
uint32_t millis(void);

void SaLTC4Init();
void SaLTC5Init();



#endif