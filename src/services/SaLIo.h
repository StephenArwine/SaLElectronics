

#ifndef _SALIO_INCLUDED
#define _SALIO_INCLUDED

#include <stdint.h>


struct IoDescriptor;

typedef int32_t (* SaLIoWrite_t)(struct IoDescriptor *const IoDescr,
                                 const uint8_t *const buf,
                                 const uint16_t length);

typedef int32_t (* SaLIoRead_t)(struct IoDescriptor *const IoDescr,
                                const uint8_t *const buf,
                                const uint16_t length);

struct IoDescriptor {
    SaLIoWrite_t write;
    SaLIoRead_t read;
};

int32_t SaLIoWrite(struct IoDescriptor *const IoDescr,
                   const uint8_t *const buf,
                   const uint16_t length);

int32_t SaLIoRead(struct IoDescriptor *const IoDescr,
                  const uint8_t *const buf,
                  const uint16_t length);


#endif