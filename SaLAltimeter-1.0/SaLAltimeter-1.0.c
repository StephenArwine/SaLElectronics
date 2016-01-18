/*
 * SaLAltimeter-1.0.c
 *
 * Created: 1/17/2016 10:58:10 PM
 * Author : Stephen Arwine
 */


#include "sam.h"
#include <samd21g18a.h>
#include <SaL.h>

#define pin1 PIN_PA12

void ClockInit() {
    // start and enable external 32k crystal
    SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ENABLE |
                           SYSCTRL_XOSC32K_XTALEN |
                           SYSCTRL_XOSC32K_EN32K |
                           ( 6 << SYSCTRL_XOSC32K_STARTUP_Pos);
    //wait for crystal to warm up
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_XOSC32KRDY)) == 0);

    GCLK->GENDIV.reg = GCLK_GENDIV_ID(1) |
                       GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(1) |
                        GCLK_GENCTRL_SRC_XOSC32K |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(1) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID_DFLL48;

    //Configure the FDLL48MHz FLL, we will use this to provide a clock to the CPU
    //Set the course and fine step sizes, these should be less than 50% of the values used for the course and fine values (P150)
    SYSCTRL->DFLLCTRL.reg = (SYSCTRL_DFLLCTRL_ENABLE); //Enable the DFLL
    SYSCTRL->DFLLMUL.reg = (SYSCTRL_DFLLMUL_CSTEP(7) | SYSCTRL_DFLLMUL_FSTEP(30));
    SYSCTRL->DFLLMUL.reg |= (SYSCTRL_DFLLMUL_MUL(1024));
    SYSCTRL->DFLLCTRL.reg |= (SYSCTRL_DFLLCTRL_MODE);

    //Wait and see if the DFLL output is good . . .
    while((SYSCTRL->PCLKSR.reg & (SYSCTRL_PCLKSR_DFLLRDY)) == 0);

    //For generic clock generator 0, select the DFLL48 Clock as input
    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_DFLL48M) | (GCLK_GENCTRL_GENEN));
    GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_GEN(0) | (GCLK_CLKCTRL_CLKEN) | GCLK_CLKCTRL_ID_DFLL48) ;
}

void PinConfig(){
	SaLPinMode(pin1,OUTPUT);
	
}

volatile uint32_t counter = 0;

int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    ClockInit();
	PinConfig();
    /* Replace with your application code */
    while (1) {
		counter++;
    }
}
