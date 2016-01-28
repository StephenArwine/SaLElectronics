

#include <SaLUSART.h>





void SaLSyncUsartIo(struct SaLUsartDescriptor *const descr,
                    struct IoDescriptor **Io) {
    *Io = &descr->io;

};