

#include <SaLUSART.h>


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

uint8_t _usartGetData(const struct _UsartDevice *const device){
	return _usartGetDataReg(device->hw);
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

    while (!_usartGetInteruptDre(&descr->device));
    do {
        _usartSetDataReg(&descr->device,buf[offset]);
        while (!_usartGetInteruptDre(&descr->device));
    } while (++offset < length);
    return offset;

}

