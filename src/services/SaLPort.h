
#ifndef SALPORT_H_
#define SALPORT_H_

//#include <SaL.h>
#include <boardDefines.h>
#include <SaLTypes.h>

static inline PortGroup* SaLGetPort(
    const uint8_t gpio_pin) {
    uint8_t port_index  = (gpio_pin / 128);
    uint8_t group_index = (gpio_pin / 32);
    /* Array of available ports */
    Port *const ports[PORT_INST_NUM] = PORT_INSTS;

    if (port_index < PORT_INST_NUM) {
        return &(ports[port_index]->Group[group_index]);
    } else {
        return NULL;
    }
};

void SaLPinMode(uint8_t pin,uint8_t DIR);

static inline void SaLDigitalOut(uint8_t pin, bool level) {
    PortGroup *const port = SaLGetPort(pin);
    uint32_t pin_mask = (1UL << (pin % 32));


    if (level) {
        port->OUTSET.reg = pin_mask;
    } else {
        port->OUTCLR.reg = pin_mask;
    }
}

static inline bool SaLDigitalIn(uint8_t pin) {
    PortGroup *const port = SaLGetPort(pin);
    uint32_t pin_mask = (1UL << (pin % 32));
    bool level = (port->IN.reg & pin_mask);
    return level;
}

typedef struct Pin {
	uint8_t  mux;
	uint8_t  group;
	uint8_t  pin;
	uint8_t  chan;
} Pin;

inline static void pin_mux(Pin p) {
	if (p.pin & 1) {
		PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXO = p.mux;
		} else {
		PORT->Group[p.group].PMUX[p.pin/2].bit.PMUXE = p.mux;
	}

	PORT->Group[p.group].PINCFG[p.pin].bit.PMUXEN = 1;
}


inline static void pin_gpio(Pin p) {
	PORT->Group[p.group].PINCFG[p.pin].bit.PMUXEN = 0;
}


inline static void pin_in(Pin p) {
	pin_gpio(p);
	PORT->Group[p.group].PINCFG[p.pin].bit.INEN = 1;
	PORT->Group[p.group].DIRCLR.reg = (1<<p.pin);
}




#endif