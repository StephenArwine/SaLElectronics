#include <SaLSample.h>


struct sample getSample(struct AltimeterModule const altimeter) {
    struct sample s;
    s.pressure			= altimeter.myAltimetersBarometer->pressure;
    s.altitude			= altimeter.myAltimetersBarometer->currentAltInFt;
    s.groundLevel		= altimeter.groundHeight;
    s.accelerationInX	= altimeter.myAltimetersAccelerometer->acceleration.Xf;
    s.accelerationInY	= altimeter.myAltimetersAccelerometer->acceleration.Yf;
    s.accelerationInZ	= altimeter.myAltimetersAccelerometer->acceleration.Zf;
    s.hour				= altimeter.myAltimetersGps->MTK3329.hour;
    s.minute			= altimeter.myAltimetersGps->MTK3329.minute;
    s.second			= altimeter.myAltimetersGps->MTK3329.second;
    s.longitude			= altimeter.myAltimetersGps->MTK3329.longitude;
    s.latitude			= altimeter.myAltimetersGps->MTK3329.latitude;
	s.GPSvalid			= altimeter.myAltimetersGps->MTK3329.validSalad; 

    return s;
}