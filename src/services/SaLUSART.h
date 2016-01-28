

#ifndef _SALUSART_H_
#define _SALUSART_H_

#include <SaLIo.h>
#include <SaL.h>

typedef uint16_t SaLSercomUsartDataReg_t;


struct _UsartDevice {
    void *hw;
};

struct SaLUsartDescriptor {
    struct IoDescriptor io;
    struct _UsartDevice device;
};

struct SaLUsartDescriptor USART_0;

static inline void SaLInitUsart(const void *const hw) {



};

static inline SaLSercomUsartDataReg_t SaLUsartDataRead(const void *const hw) {
    return ((Sercom *)hw)->USART.DATA.reg;
};

static inline void SaLUsartDataWrite(const void *const hw,SaLSercomUsartDataReg_t data) {

    ((Sercom *)hw)->USART.DATA.reg = data;

};


#endif