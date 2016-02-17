
#ifndef USBSERIAL_H_ 
#define USBSERIAL_H_





void usbserial_init();
void usbserial_out_completion();
void usbserial_dma_tx_completion();
void usbserial_rx_flush();
void usbserial_dma_rx_completion();
void usbserial_handle_tc();
void usbserial_in_completion();
void usbserial_disable();
#endif