
#ifndef SALDELAY_H_
#define SALDELAY_H_

#include <SaL.h>


void SaLDelayInit(void);

static inline void delay_cycles(
    const uint32_t n) {
    if (n > 0) {
        SysTick->LOAD = n;
        SysTick->VAL = 0;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
        };
    }
}


/**
 * \def delay_ms
 * \brief Delay in at least specified number of milliseconds.
 * \param delay Delay in milliseconds
 */
void delay_ms(uint32_t delay);

/**
 * \def delay_us
 * \brief Delay in at least specified number of microseconds.
 * \param delay Delay in microseconds
 */
void delay_us(uint32_t delay);



#endif /* DELAY_H_INCLUDED */




