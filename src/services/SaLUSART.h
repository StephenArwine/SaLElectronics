

#ifndef _SALUSART_H_
#define _SALUSART_H_


#include <SaLIo.h>

typedef uint16_t SaLSercomUsartDataReg_t;

static inline void uart_sync(void) {
    while (SERCOM5->USART.SYNCBUSY.bit.CTRLB);
}

void uart_init(uint32_t baud);

struct _UsartDevice {
    void *hw;
};

struct SaLUsartDescriptor {
    struct IoDescriptor io;
    struct _UsartDevice device;
};

struct SaLUsartDescriptor USART_0;

void SaLSyncUsartIo(struct SaLUsartDescriptor *const descr,
                    struct IoDescriptor **Io);

static inline void _usartSetDataReg(const void *const hw, uint8_t data ) {
    ATOMIC_SECTION_ENTER
    ((Sercom *)hw)->USART.DATA.reg = data;
    ATOMIC_SECTION_LEAVE
}

static inline SaLSercomUsartDataReg_t _usartGetDataReg(const void *const hw) {
    return     ((Sercom *)hw)->USART.DATA.reg;
}

static inline bool _usartGetInteruptRCX(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_RXC) >> SERCOM_USART_INTFLAG_RXC_Pos;

};
static inline bool _usartGetDreInteruptReg(const void *const hw) {
    return (((Sercom *)hw)->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE) >> SERCOM_USART_INTFLAG_DRE_Pos;
};

int32_t SaLInitUsart(struct SaLUsartDescriptor *const descr,
                     void *const hw);
#endif