#include <SaLUsb.h>









static void usbLoadPadCalib(void) {
#define NVM_USB_PAD_TRANSN_POS  45
#define NVM_USB_PAD_TRANSN_SIZE 5
#define NVM_USB_PAD_TRANSP_POS  50
#define NVM_USB_PAD_TRANSP_SIZE 5
#define NVM_USB_PAD_TRIM_POS  55
#define NVM_USB_PAD_TRIM_SIZE 3
    Usb *hw = USB;
    uint32_t pad_transn =( *((uint32_t *)(NVMCTRL_OTP4)
                             + (NVM_USB_PAD_TRANSN_POS / 32))
                           >> (NVM_USB_PAD_TRANSN_POS % 32))
                         & ((1 << NVM_USB_PAD_TRANSN_SIZE) - 1);
    uint32_t pad_transp =( *((uint32_t *)(NVMCTRL_OTP4)
                             + (NVM_USB_PAD_TRANSP_POS / 32))
                           >> (NVM_USB_PAD_TRANSP_POS % 32))
                         & ((1 << NVM_USB_PAD_TRANSP_SIZE) - 1);
    uint32_t pad_trim =( *((uint32_t *)(NVMCTRL_OTP4)
                           + (NVM_USB_PAD_TRIM_POS / 32))
                         >> (NVM_USB_PAD_TRIM_POS % 32))
                       & ((1 << NVM_USB_PAD_TRIM_SIZE) - 1);
    if (pad_transn == 0x1F) {
        pad_transn = 5;
    }
    if (pad_transp == 0x1F) {
        pad_transp = 29;
    }
    if (pad_trim == 0x7) {
        pad_trim = 5;
    }
    hw->DEVICE.PADCAL.reg = USB_PADCAL_TRANSN(pad_transn) |
                            USB_PADCAL_TRANSP(pad_transp) | USB_PADCAL_TRIM(pad_trim);
}

int32_t usbDeviceEnable(void) {
    Usb *hw = USB;
    //uint8_t ctrla;

    ((Usb *)hw)->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;
    NVIC_EnableIRQ(USB_IRQn);

    ((Usb *)hw)->DEVICE.INTENSET.reg = USB_DEVICE_INTENSET_SOF |
                                       USB_DEVICE_INTENSET_EORST |
                                       USB_DEVICE_INTENSET_RAMACER |
                                       USB_DEVICE_INTENSET_LPMSUSP |
                                       USB_DEVICE_INTENSET_SUSPEND;

    ((Usb *)hw)->DEVICE.CTRLB.reg &= ~USB_DEVICE_CTRLB_DETACH;

return 0;
}

int32_t usbDeviceStart() {

    usbDeviceEnable();
    return ERR_NONE;
}


void UsbInit() {

    Usb *hw = USB;

//pm stuff first
    PM->APBBMASK.reg |= PM_APBBMASK_USB;
    PM->AHBMASK.reg |= PM_AHBMASK_USB;

//gclk init stuff

    GCLK->GENDIV.reg = GCLK_GENDIV_ID(7) |
                       GCLK_GENDIV_DIV(1);

    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(7) |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_SRC_DFLL48M |
                        GCLK_GENCTRL_GENEN;

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_USB |
                        GCLK_CLKCTRL_GEN(7) |
                        GCLK_CLKCTRL_CLKEN;

    UsbWaitForSync(hw,USB_SYNCBUSY_SWRST);
    if (ubsGetCtrlAEnableBit(hw)) {
    }

    //set usb crtla swrst bit
    UsbWaitForSync(hw,USB_SYNCBUSY_SWRST);
    ATOMIC_SECTION_ENTER
    hw->DEVICE.CTRLA.reg |= USB_CTRLA_SWRST;
    ATOMIC_SECTION_LEAVE

    usbLoadPadCalib();

//set run in standby bit
    UsbWaitForSync(hw,USB_SYNCBUSY_SWRST);
    ATOMIC_SECTION_ENTER
    hw->DEVICE.CTRLA.reg |= USB_CTRLA_RUNSTDBY;
    ATOMIC_SECTION_LEAVE

//write discriptor address
    UsbWaitForSync(hw,USB_SYNCBUSY_SWRST);
    ATOMIC_SECTION_ENTER
    ((Usb *)hw)->DEVICE.DESCADD.reg = 0x20000084;
    ATOMIC_SECTION_LEAVE

    ATOMIC_SECTION_ENTER
    ((Usb *)hw)->DEVICE.DADD.reg = USB_DEVICE_DADD_DADD(1) |
                                   USB_DEVICE_DADD_ADDEN;
    ATOMIC_SECTION_LEAVE

    ATOMIC_SECTION_ENTER
    ((Usb *)hw)->DEVICE.CTRLB.reg = 1;
    ATOMIC_SECTION_LEAVE

    ((Port *)PORT)->Group[0].DIR.reg |= (1UL << 24);
    ((Port *)PORT)->Group[0].DIR.reg |= (1UL << 25);

    ((Port *)PORT)->Group[0].PINCFG[24].reg = 0x41;
    ((Port *)PORT)->Group[0].PINCFG[25].reg = 0x41;

    ((Port *)PORT)->Group[0].PMUX[12].reg = 0x66;


    usbDeviceStart();
    usbDeviceEnable();



}