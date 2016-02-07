
#ifndef SALUSB_H_
#define SALUSB_H_

#include <SaLTypes.h>
#include <SaL.h>


typedef uint8_t UsbSyncBusyReg_t;

/** USB Device endpoint callback type. */
enum usb_d_ep_cb_type {
	/** USB device endpoint setup callback. */
	USB_D_EP_CB_SETUP,
	/** USB device endpoint more data callback. */
	USB_D_EP_CB_MORE,
	/** USB device endpoint transaction done or error callback. */
	USB_D_EP_CB_XFER,
	/** Number of types of USB device endpoint callback types. */
	USB_D_EP_CB_N
};


/** Describes a list of USB descriptors. */
struct usbDeviceDescriptors {
    /** Pointer to Start of Descriptors. */
    uint8_t *sod;
    /** Pointer to End of Descriptors. */
    uint8_t *eod;
};

/** Describes the USB device core descriptors. */
struct usbDeviceCoreDescriptors {
	struct usbDeviceDescriptors *ls_fs;
};


static inline void UsbWaitForSync(const void *const hw, UsbSyncBusyReg_t reg) {
    while (((Usb *)hw)->DEVICE.SYNCBUSY.reg & reg) {};
}

static inline bool ubsGetCtrlAEnableBit(const void *const hw) {
    uint8_t tmp;
    UsbWaitForSync(hw, USB_SYNCBUSY_SWRST|USB_SYNCBUSY_ENABLE);
    tmp = ((Usb *)hw)->DEVICE.CTRLA.reg;
    tmp = (tmp & USB_CTRLA_ENABLE) >> USB_CTRLA_ENABLE_Pos;
    return (bool)tmp;
}



void UsbInit();








#endif