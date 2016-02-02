#include <SaLSample.h>

bool baroSampleReady;
bool accelSampleReady;
bool gpsSampleReady;

struct sample sampleBeingCooked;
struct sample cookedSample;


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

uint16_t ticks = 0;
volatile uint16_t mil;
volatile uint16_t mil2;

void sampleTick() {

    if (!baroSampleReady) baroSampleReady = baroSample();
    if (!accelSampleReady) accelSampleReady = accelSample();


    if (baroSampleReady && accelSampleReady) {
        ticks++;

        sampleBeingCooked.altitude = SaLBaroGetHeight();
        sampleBeingCooked.accelerationInX = SaLGetAccelX();
        sampleBeingCooked.accelerationInY = SaLGetAccelY();
        sampleBeingCooked.accelerationInZ = SaLGetAccelZ();
        baroSampleState = 0;
        accelSampleState = 0;
        baroSampleReady = false;
        accelSampleReady = false;
    }

    if (ticks > 1000) {
        //  SaLPlayTone(400);
        mil = millis();
        mil2 = millis();

        ticks = 0;
    }
}

void sampleInit() {

    baroSampleReady = false;
    accelSampleReady = false;
    gpsSampleReady = false;

}
