
#include <SaLClocks.h>


void SalGclkInit() {

    SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
                           SYSCTRL_INTFLAG_DFLLRDY;
    NVMCTRL->CTRLB.reg |= NVMCTRL_CTRLB_RWS_HALF;

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

// gclk for tc3
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

#define NVM_DFLL_COARSE_POS    58
#define NVM_DFLL_COARSE_SIZE   6
#define NVM_DFLL_FINE_POS      64
#define NVM_DFLL_FINE_SIZE     10
    uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP4)
                         + (NVM_DFLL_COARSE_POS / 32))
                       >> (NVM_DFLL_COARSE_POS % 32))
                     & ((1 << NVM_DFLL_COARSE_SIZE) - 1);
    if (coarse == 0x3f) {
        coarse = 0x1f;
    }
    uint32_t fine =( *((uint32_t *)(NVMCTRL_OTP4)
                       + (NVM_DFLL_FINE_POS / 32))
                     >> (NVM_DFLL_FINE_POS % 32))
                   & ((1 << NVM_DFLL_FINE_SIZE) - 1);
    if (fine == 0x3ff) {
        fine = 0x1ff;
    }
//For generic clock generator 0, select the DFLL48 Clock as input
    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(1));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN));
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_CLKEN ) ;

    SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_MODE) |
                             SYSCTRL_DFLLCTRL_USBCRM |
                             SYSCTRL_DFLLCTRL_ENABLE;

    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(1) | SYSCTRL_DFLLMUL_FSTEP(1));
    SYSCTRL->DFLLMUL.reg |= (SYSCTRL_DFLLMUL_MUL(48000000/32768));

//Wait and see if the DFLL output is good . . .
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);


    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(4) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4) |
                        GCLK_GENCTRL_SRC_OSC8M |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TC4_TC5 |
                        GCLK_CLKCTRL_GEN_GCLK4 |
                        GCLK_CLKCTRL_CLKEN;

}

void SaLTC4Init() {

    PM->APBCMASK.reg |= PM_APBCMASK_TC4;

    TC4->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                             TC_CTRLA_RUNSTDBY |
                             TC_CTRLA_PRESCALER_DIV8;
    //  TC3->COUNT16.PER.reg = 0xFE;

    TC4->COUNT16.INTENSET.reg = TC_INTENSET_OVF;

    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    NVIC_EnableIRQ(TC4_IRQn);
}

// void SaLTC5Init() {
//
//     PM->APBCMASK.reg |= PM_APBCMASK_TC5;
//
//     TC5->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
//                             TC_CTRLA_RUNSTDBY |
//                             TC_CTRLA_PRESCALER_DIV256;
//     TC5->COUNT8.PER.reg = 0xFE;
//
//     TC5->COUNT8.INTENSET.reg = TC_INTENSET_OVF;
//
//     TC5->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
//
//     NVIC_EnableIRQ(TC5_IRQn);
// }



void SaLRtcInit() {
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);

    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                        GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_XOSC32K) |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

// Configure RTC
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(RTC_GCLK_ID) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(2);

    RTC->MODE1.CTRL.reg = RTC_MODE1_CTRL_MODE_COUNT16 |
                          RTC_MODE1_CTRL_PRESCALER_DIV1;
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

volatile uint32_t time_ms = 0;

void RTC_Handler(void) {
    time_ms += 1000;
    RTC->MODE1.INTFLAG.reg = 0xFF;
}

uint32_t millis(void) {
    uint32_t ms;
    ATOMIC_SECTION_ENTER
    ms = time_ms + RTC->MODE1.COUNT.reg;
    if (RTC->MODE1.INTFLAG.bit.OVF)
        ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
    ATOMIC_SECTION_LEAVE
    return ms;
}

