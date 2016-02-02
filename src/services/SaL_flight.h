#ifndef SAL_FLIGHT_H_
#define SAL_FLIGHT_H_

#include <SaL.h>

enum SaL_flight_state {
    SaL_flight_startup = 0,
    SaL_flight_pad = 1,
    SaL_flight_boost = 2,
    SaL_flight_coast = 3,
    SaL_flight_drogue = 4,
    SaL_flight_main = 5,
    SaL_flight_landed = 6,
	SaL_flight_error = 7,
	SaL_flight_program = 8
};

extern enum SaL_flight_state SaL_flight_state;

void Sal_flight(void);


void SaL_flight_init(void);

#endif
