
#include <SaLAnalog.h>


void adcInit() {

    // set up clock
    PM->APBCMASK.reg |= PM_APBCMASK_ADC;

    // divide prescaler by 512 (93.75KHz), max adc freq is 2.1MHz
    ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV512 |
                     ADC_CTRLB_RESSEL_8BIT;

    // enable clock adc channel
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(0) |
                        GCLK_CLKCTRL_ID(ADC_GCLK_ID);

    ADC->CALIB.reg =
        ADC_CALIB_BIAS_CAL(
            (*(uint32_t *)ADC_FUSES_BIASCAL_ADDR >> ADC_FUSES_BIASCAL_Pos)
        ) |
        ADC_CALIB_LINEARITY_CAL(
            (*(uint64_t *)ADC_FUSES_LINEARITY_0_ADDR >> ADC_FUSES_LINEARITY_0_Pos)
        );

    ADC->REFCTRL.reg = ADC_REFCTRL_REFSEL_INTVCC1;

    ADC->CTRLA.reg = ADC_CTRLA_ENABLE; // enable
    while(ADC->STATUS.reg & ADC_STATUS_SYNCBUSY);
}

uint16_t adcSample() {
    ADC->SWTRIG.reg = ADC_SWTRIG_START;
    while(!(ADC->INTFLAG.reg & ADC_INTFLAG_RESRDY)); // wait until result is ready
    return ADC->RESULT.reg;
}

uint16_t adcRead(Pin p) {
    // switch pin mux to analog in
    pinAnalog(p);

    ADC->INPUTCTRL.reg = (ADC_INPUTCTRL_MUXPOS(p.chan) // select from proper pin
                          | ADC_INPUTCTRL_MUXNEG_GND // 0 = gnd
                          | ADC_INPUTCTRL_GAIN_1X);

    return adcSample();
}