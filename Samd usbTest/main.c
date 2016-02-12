/*
 * Samd usbTest.c
 *
 * Created: 2/11/2016 11:51:13 AM
 * Author : sal
 */



#include "sam.h"
#include <usb_samd.h>
#include <fw.h>



// Configure DFLL in USB recovery mode
const uint32_t dfll_ctrl_usb
    = SYSCTRL_DFLLCTRL_ENABLE
      | SYSCTRL_DFLLCTRL_CCDIS
      | SYSCTRL_DFLLCTRL_BPLCKC
      | SYSCTRL_DFLLCTRL_USBCRM
      | SYSCTRL_DFLLCTRL_ONDEMAND;

void initclocks() {

    SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
                           SYSCTRL_INTFLAG_DFLLRDY;

    NVMCTRL->CTRLB.bit.RWS = 2;

    // Initialize GCLK
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while (GCLK->CTRL.reg & GCLK_CTRL_SWRST);

    // SERCOM slow clock (Shared by all SERCOM)
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(0) |
                        GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_SLOW);


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


    // Disable ONDEMAND mode while writing configurations (errata 9905)
    SYSCTRL->DFLLCTRL.reg = dfll_ctrl_usb & ~SYSCTRL_DFLLCTRL_ONDEMAND;
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);
    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(1) | SYSCTRL_DFLLMUL_FSTEP(1));
    SYSCTRL->DFLLMUL.reg |= SYSCTRL_DFLLMUL_MUL(1465); // round(48000000 / 32768)

//Wait and see if the DFLL output is good . . .
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);

    SYSCTRL->DFLLCTRL.reg = dfll_ctrl_usb;

//For generic clock generator 0, select the DFLL48 Clock as input
    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN));

}

int main(void) {

    initclocks();

   // ((Port *)PORT)->Group[0].DIR.reg |= (1UL << 24);
 //   ((Port *)PORT)->Group[0].DIR.reg |= (1UL << 25);
    ((Port *)PORT)->Group[0].PINCFG[24].reg = 0x41;
    ((Port *)PORT)->Group[0].PINCFG[25].reg = 0x41;
//   ((Port *)PORT)->Group[0].PMUX[12].reg = 0x66;

    pin_mux(PIN_USB_DM);
    pin_mux(PIN_USB_DP);

    usb_init();
    usb_attach();
    NVIC_SetPriority(USB_IRQn, 0xff);


    volatile uint32_t count = 0;
    /* Replace with your application code */
    while (1) {
        count++;
    }
}
