

#include <SaLUSART.h>

void uart_init(uint32_t baud) {

    uint32_t UART_CLKGEN_F = 8000000UL;
    uint64_t br = (uint64_t)65536 * (UART_CLKGEN_F - 16 * baud) / UART_CLKGEN_F;

    //enable GPS pins
    //  SaLPinMode(MTK3339_RX_PIN,INPUT);
    //  SaLPinMode(MTK3339_TX_PIN,OUTPUT);
    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_ENABLE;
    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_PRESC_3;
    SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;



    //portB22->PINCFG->reg = 0x44;
    // portB23->PINCFG->reg = 0x44;

    // GPS pin configs
    ((Port *)PORT)->Group[1].PINCFG[22].reg = 0x41;
    ((Port *)PORT)->Group[1].PINCFG[23].reg = 0x41;
    ((Port *)PORT)->Group[1].PMUX[11].reg = 0x32;


    // usb port configs
  //  ((Port *)PORT)->Group[0].PINCFG[24].reg = 0x41;
   // ((Port *)PORT)->Group[0].PINCFG[25].reg = 0x41;
   // ((Port *)PORT)->Group[0].PMUX[12].reg = 0x24;


    //enable power to sercom 5 module
    PM->APBCMASK.reg |= PM_APBCMASK_SERCOM5;
    //enable and configure the sercom clock
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(3) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(3) |
                        GCLK_GENCTRL_SRC_OSC8M |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM5_CORE |
                        GCLK_CLKCTRL_GEN_GCLK3 |
                        GCLK_CLKCTRL_CLKEN;
    //     GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOMX_SLOW |
    //                         GCLK_CLKCTRL_GEN_GCLK3 |
    //                         GCLK_CLKCTRL_CLKEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM3_CORE |
                        GCLK_CLKCTRL_GEN_GCLK3 |
                        GCLK_CLKCTRL_CLKEN;

    //configure the sercom module for the gps (sercom 5)
    SERCOM5->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD |
                               SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
                               SERCOM_USART_CTRLA_RXPO(3) |
                               SERCOM_USART_CTRLA_TXPO(1);
    uart_sync(SERCOM5);
    SERCOM5->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN |
                               SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);
    // SERCOM_USART_CTRLB_SFDE;
    uart_sync(SERCOM5);
    SERCOM5->USART.BAUD.reg = (uint16_t)br;
    uart_sync(SERCOM5);
    SERCOM5->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
    uart_sync(SERCOM5);

    SaLInitUsart(&USART_0,SERCOM5);


}
static int32_t SaLUsartDataRead(struct IoDescriptor *const IoDescr,
                                uint8_t *const buf,
                                const uint16_t length);
static int32_t SaLUsartDataWrite(struct IoDescriptor *const ioDescr,
                                 const uint8_t *const buf,
                                 const uint16_t length);

int32_t SaLInitUsart(struct SaLUsartDescriptor *const descr,
                     void *const hw) {

    descr->device.hw = hw;
    descr->io.read = SaLUsartDataRead;
    descr->io.write = SaLUsartDataWrite;
    return (int32_t)NULL;
};


void SaLSyncUsartIo(struct SaLUsartDescriptor *const descr,
                    struct IoDescriptor **Io) {
    *Io = &descr->io;
};

bool _usartByteRecieved(const struct _UsartDevice *const device) {
    return _usartGetInteruptRCX(device->hw);
};

uint8_t _usartGetData(const struct _UsartDevice *const device) {
    return _usartGetDataReg(device->hw);
}

bool _usartGetDreInterupt(const struct _UsartDevice *const device) {
    return _usartGetDreInteruptReg(device->hw);
}
void _usartSetData(const struct _UsartDevice *const device, uint8_t data) {
    _usartSetDataReg(device->hw,data);
}




static int32_t SaLUsartDataRead(struct IoDescriptor *const ioDescr,
                                uint8_t *const buf,
                                const uint16_t length) {

    int32_t offset = 0;
    struct SaLUsartDescriptor *descr = CONTAINER_OF(ioDescr,struct SaLUsartDescriptor, io);

    do {
        while (!_usartByteRecieved(&descr->device));
        buf[offset] = _usartGetData(&descr->device);
    } while (++offset < length);

    return (int32_t)offset;
}

static int32_t SaLUsartDataWrite(struct IoDescriptor *const ioDescr,
                                 const uint8_t *const buf,
                                 const uint16_t length) {

    int32_t offset = 0;
    struct SaLUsartDescriptor *descr = CONTAINER_OF(ioDescr,struct SaLUsartDescriptor, io);

    while (!_usartGetDreInterupt(&descr->device));
    do {
        _usartSetDataReg(&descr->device,buf[offset]);
        while (!_usartGetDreInterupt(&descr->device));
    } while (++offset < length);
    return offset;

}

