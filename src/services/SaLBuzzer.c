
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
                        GCLK_GENCTRL_SRC_XOSC32K |
                        GCLK_GENCTRL_GENEN;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(0) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_ID(TC5_GCLK_ID);


    TC5->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16|
                             TC_CTRLA_RUNSTDBY |
                             TC_CTRLA_PRESCALER_DIV1;

    TC5->COUNT16.CC[0].bit.CC = 260;

    //TC5->COUNT16.PER.reg = 0x02;

    TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC0;

    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    NVIC_EnableIRQ(TC5_IRQn);
}

void TC5_Handler() {
    TC5->COUNT16.INTFLAG.reg = 0XFF;
    TC5->COUNT16.COUNT.bit.COUNT = 0;
    pinToggle(BUZZER);
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