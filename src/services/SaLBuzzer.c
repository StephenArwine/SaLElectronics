
#include <SaLBuzzer.h>

void SaLBuzzerInit() {

    pinOut(BUZZER);
    pinCfg(BUZZER);
//
    PM->APBCMASK.reg |= PM_APBCMASK_TC5;
//
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(5) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(5) |
                        GCLK_GENCTRL_SRC_OSC8M |
                        GCLK_GENCTRL_GENEN;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID(TC5_GCLK_ID);


    TC5->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16|
                             TC_CTRLA_RUNSTDBY |
                             //	 TC_CTRLA_PRESCSYNC_GCLK |
                             TC_CTRLA_PRESCALER_DIV4;

    // TC5->COUNT16.EVCTRL.bit.EVACT = 0x1;

    TC5->COUNT16.CC[0].bit.CC = 700;

    //TC5->COUNT16.PER.reg = 0x02;

    TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC0;

    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    TC5->COUNT16.EVCTRL.bit.EVACT = 0x1;


    NVIC_EnableIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn,0xFFF);

    TC5->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_STOP;
}

uint16_t buzzes = 0;

void TC5_Handler() {

    TC5->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
    TC5->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_RETRIGGER;
    pinToggle(BUZZER);
    buzzes++;
    if (buzzes > 4000) {
        TC5->COUNT16.CTRLA.reg = 0;
        TC5->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_STOP;
        buzzes = 0;
        pinLow(BUZZER);
    }

}

void SaLPlayTone(int16_t tone_,int16_t durration) {


    int32_t elapsed_time = 0;


    while (elapsed_time < (durration*100)) {

        pinToggle(BUZZER);
        delay_us(tone_/2);

        pinToggle(BUZZER);
        delay_us(tone_/2);


        elapsed_time += (tone_);


    }
}

uint32_t milliseconds;
uint32_t lastbuzz;
uint16_t toneticks = 1000;

void toneTick() {

    if (toneticks > 0) {

        if (milliseconds-lastbuzz > 3) {
            toneticks--;
            pinToggle(BUZZER);
			lastbuzz = milliseconds;
        }
    }
}


