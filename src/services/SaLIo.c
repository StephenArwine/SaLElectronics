
#include <SaLIo.h>


int32_t SaLIoWrite(struct IoDescriptor *const IoDescr,
                   const uint8_t *const buf,
                   const uint16_t length) {
    return IoDescr->write(IoDescr,buf,length);
}

int32_t SaLIoRead(struct IoDescriptor *const IoDescr,
                  const uint8_t *const buf,
                  const uint16_t length) {
    return IoDescr->read(IoDescr,buf,length);
};

