
#ifndef MTK3329_H_
#define MTK3329_H_

#include <stdlib.h>
#include <SaLTypes.h>
//#include <SaL.h>


 struct MTK3329Module {
    float timef;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float longitude;
    float latitude;
	bool validSalad;

};

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"

#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"

#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"

void MTK3329ParseGPRMC(struct MTK3329Module *const myMTK3329,uint8_t *const RMCMessage);

void MTK3329ParseMessage(struct MTK3329Module *const myMTK3329,uint8_t *const messageToParse);

#endif