
#ifndef MTK3329_H_
#define MTK3329_H_
#include <SaL.h>
#include <stdlib.h>


typedef struct MTK3329MODULE {
    float timef;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float longitude;
    float latitude;
	bool validSalad;

} MTK3329;

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"

#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"

void MTK3329ParseGPRMC(MTK3329 *const myMTK3329,uint8_t *const RMCMessage);

void MTK3329ParseMessage(MTK3329 *const myMTK3329,uint8_t *const messageToParse);

#endif