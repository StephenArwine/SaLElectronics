

#ifndef _SALIO_H_
#define _SALIO_H_

#include <stdint.h>
#include <stdbool.h>
#include <SaL.h>



struct IoDescriptor;

typedef int32_t (* SaLIoWrite_t)(struct IoDescriptor *const IoDescr,
                                 const uint8_t *const buf,
                                 const uint16_t length);

typedef int32_t (* SaLIoRead_t)(struct IoDescriptor *const IoDescr,
                                uint8_t *const buf,
                                const uint16_t length);

struct IoDescriptor {
    SaLIoWrite_t write;
    SaLIoRead_t read;
};

int32_t SaLIoWrite(struct IoDescriptor *const IoDescr,
                   const uint8_t *const buf,
                   const uint16_t length);

int32_t SaLIoRead(struct IoDescriptor *const IoDescr,
                  uint8_t *const buf,
                  const uint16_t length);

#endif