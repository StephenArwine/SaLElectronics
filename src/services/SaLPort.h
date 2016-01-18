
#ifndef SALPORT_H_
#define SALPORT_H_

#include <SaL.h>

#define INPUT 0
#define OUTPUT 1

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

static inline void SaLDigitalOut(uint8_t pin, uint8_t level) {
	PortGroup *const port = SaLGetPort(pin);
	uint32_t pin_mask = (1UL << (pin % 32));

	if (level) {
		port->OUTSET.reg = pin_mask;
		} else {
		port->OUTCLR.reg = pin_mask;
	}
};

static inline bool SaLDigitalIn(uint8_t pin) {
	PortGroup *const port = SaLGetPort(pin);
	uint32_t pin_mask = (1UL << (pin % 32));
	bool level = (port->IN.reg & pin_mask);
	return level;
}


#endif