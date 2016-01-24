/*
 * SaLAltimeter_rtc_test.c
 *
 * Created: 1/24/2016 10:45:22 AM
 * Author : Stephen Arwine
 */ 


#include <samd21g18a.h>

#define ATOMIC_SECTION_ENTER   { register uint32_t __atomic; \
	__asm volatile ("mrs %0, primask" : "=r" (__atomic) ); \
	__asm volatile ("cpsid i");
#define ATOMIC_SECTION_LEAVE   __asm volatile ("msr primask, %0" : : "r" (__atomic) ); }


void RtcInit(){
	 SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE | SYSCTRL_XOSC32K_EN32K;
	 //wait for crystal to warm up
	 while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_XOSC32KRDY)) == 0);
	 SYSCTRL->OSC8M.reg = SYSCTRL_OSC8M_ENABLE;
	 GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);
	 GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K) |
	 GCLK_GENCTRL_IDC | GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
	 while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
	 // Configure RTC
	 GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(RTC_GCLK_ID) |
	 GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(2);
	 RTC->MODE1.CTRL.reg = RTC_MODE1_CTRL_MODE_COUNT16;
	 while (RTC->MODE1.STATUS.bit.SYNCBUSY);
	 // Prescaler needs to be enabled separately from the mode for some reason
	 RTC->MODE1.CTRL.reg |= RTC_MODE1_CTRL_PRESCALER_DIV32;
	 while (RTC->MODE1.STATUS.bit.SYNCBUSY);
	 RTC->MODE1.PER.reg = 998;
	 while (RTC->MODE1.STATUS.bit.SYNCBUSY);
	 RTC->MODE1.READREQ.reg |= RTC_READREQ_RCONT | RTC_READREQ_ADDR(0x10);
	 RTC->MODE1.INTENSET.reg = RTC_MODE1_INTENSET_OVF;
	 RTC->MODE1.CTRL.bit.ENABLE = 1;
	 while (RTC->MODE1.STATUS.bit.SYNCBUSY);
	 NVIC_EnableIRQ(RTC_IRQn);

}

void RTC_Handler(void) {
	time_ms += 1000;
	RTC->MODE1.INTFLAG.bit.OVF = 1;
}

static uint32_t millis(void) {
	uint32_t ms;
	ATOMIC_SECTION_ENTER
	ms = time_ms + RTC->MODE1.COUNT.reg;
	if (RTC->MODE1.INTFLAG.bit.OVF)
	ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
	ATOMIC_SECTION_LEAVE
	return ms;
}


int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

    /* Replace with your application code */
    while (1) 
    {
    }
}
