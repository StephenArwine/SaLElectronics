
#ifndef SALSAMPLE_H_
#define SALSAMPLE_H_

#include <SaLAltimeter.h>

struct sample {

    uint32_t pressure;
    float altitude;
    float groundLevel;
    float accelerationInX;
    float accelerationInY;
    float accelerationInZ;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float longitude;
    float latitude;
	bool GPSvalid;

};

/* returns a current data sample from the altimeters sensors */
struct sample getSample(struct AltimeterModule const altimeter) ;


#endif