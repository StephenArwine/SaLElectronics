#include <SaLSample.h>

bool baroSampleReady;
bool accelSampleReady;
bool gpsSampleReady;

struct sample sampleBeingCooked;
struct sample cookedSample;
uint32_t sampleMills;


struct sample getSample(struct AltimeterModule altimeter) {
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

int16_t index222 = 0;
uint16_t thisTime;
uint16_t lastTime;
uint16_t deltatt;

bool sampleTick() {

    if (!baroSampleReady) baroSampleReady = baroSample();
    if (!accelSampleReady) accelSampleReady = accelSample();


    if (baroSampleReady && accelSampleReady) {

        sampleBeingCooked.sampleMills = millis();
        sampleBeingCooked.deltaT = sampleBeingCooked.sampleMills - cookedSample.sampleMills;
        sampleBeingCooked.altitude = SaLBaroGetHeight();
        sampleBeingCooked.accelerationInX = SaLGetAccelX();
        sampleBeingCooked.accelerationInY = SaLGetAccelY();
        sampleBeingCooked.accelerationInZ = SaLGetAccelZ();
        sampleBeingCooked.position = SaLBaroGetHeight();

        //  sampleBeingCooked.velocityStar = cookedSample.positionStar + sampleBeingCooked.accelerationInX*sampleBeingCooked.deltaT;
        //  sampleBeingCooked.positionStar = cookedSample.position +
        //                                 cookedSample.velocityStar * sampleBeingCooked.deltaT +
        //                                   sampleBeingCooked.accelerationInZ *  (pow(sampleBeingCooked.deltaT,2)/2);
        index222++;

        baroSampleState = 0;
        accelSampleState = 0;
        baroSampleReady = false;
        accelSampleReady = false;
        cookedSample = sampleBeingCooked;


        if (index222 > 1000) {
            index222 = 0;
            thisTime = millis();
            deltatt = thisTime - lastTime;
            lastTime = millis();
			            //SaLPlayTone(700,100);

        }
		return true;
    } else {
		return false;
	}
}

void sampleInit() {

    baroSampleReady = false;
    accelSampleReady = false;
    gpsSampleReady = false;
    cookedSample.sampleMills = millis();
}
