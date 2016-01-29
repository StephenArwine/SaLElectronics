
#include <SaLClocks.h>

void SalGclkInit() {

    SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
                           SYSCTRL_INTFLAG_DFLLRDY;
    NVMCTRL->CTRLB.bit.RWS = 1;

// start and enable external 32k crystal
    SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE |
                           SYSCTRL_XOSC32K_XTALEN |
                           SYSCTRL_XOSC32K_EN32K |
                           ( 6 << SYSCTRL_XOSC32K_STARTUP_Pos);
//wait for crystal to warm up
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_XOSC32KRDY)) == 0);

//config xosc32k for the dfll via gen1
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(1) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(1) |
                        GCLK_GENCTRL_SRC_XOSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(1) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID_DFLL48;

    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(2) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                        GCLK_GENCTRL_SRC_OSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(2) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID(TC3_GCLK_ID);

//Configure the FDLL48MHz FLL, we will use this to provide a clock to the CPU
//Set the course and fine step sizes, these should be less than 50% of the values used for the course and fine values (P150)
    SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_ENABLE); //Enable the DFLL
    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(7) | SYSCTRL_DFLLMUL_FSTEP(30));
    SYSCTRL->DFLLMUL.reg |= (SYSCTRL_DFLLMUL_MUL(1280));
    SYSCTRL->DFLLCTRL.reg |= (SYSCTRL_DFLLCTRL_MODE);
//Wait and see if the DFLL output is good . . .
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);
//For generic clock generator 0, select the DFLL48 Clock as input
    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN));
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_CLKEN ) ;
//set up OSC8M

}

void SaLRtcInit() {
GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);

GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K) |
GCLK_GENCTRL_IDC | GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

// Configure RTC
GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(RTC_GCLK_ID) |
GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(2);

RTC->MODE1.CTRL.reg = RTC_MODE1_CTRL_MODE_COUNT16 |
RTC_MODE1_CTRL_PRESCALER_DIV32;
while (RTC->MODE1.STATUS.bit.SYNCBUSY);

// Prescaler needs to be enabled separately from the mode for some reason
//  RTC->MODE1.CTRL.reg |= RTC_MODE1_CTRL_PRESCALER_DIV1;
while (RTC->MODE1.STATUS.bit.SYNCBUSY);

RTC->MODE1.PER.reg = 998;
while (RTC->MODE1.STATUS.bit.SYNCBUSY);

RTC->MODE1.READREQ.reg |= RTC_READREQ_RCONT | RTC_READREQ_ADDR(0x10);

RTC->MODE1.INTENSET.reg = RTC_MODE1_INTENSET_OVF;

RTC->MODE1.CTRL.bit.ENABLE = 1;
while (RTC->MODE1.STATUS.bit.SYNCBUSY);

NVIC_EnableIRQ(RTC_IRQn);
}