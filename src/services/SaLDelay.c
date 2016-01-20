#include <SaLDelay.h>

static uint32_t cycles_per_ms = 48000000UL / 1000;
static uint32_t cycles_per_us = 48000000UL / 1000000;

void SaLDelayInit(void) {
    cycles_per_ms = 41900000UL;
    cycles_per_ms /= 1000;
    cycles_per_us = cycles_per_ms / 1000;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

/**
 * \brief Delay loop to delay at least n number of microseconds
 *
 * \param n  Number of microseconds to wait
 */
void delay_us(
    uint32_t n) {
    while (n--) {
        /* Devide up to blocks of 10u */
        delay_cycles(cycles_per_us);
    }
}

/**
 * \brief Delay loop to delay at least n number of milliseconds
 *
 * \param n  Number of milliseconds to wait
 */
void delay_ms(
    uint32_t n) {
    while (n--) {
        /* Devide up to blocks of 1ms */
        delay_cycles(cycles_per_ms);
    }
}