

#include <SaLUSART.h>



void configUSARTModule( struct USARTModule *const module,
uint8_t TX_PIN,
uint8_t RX_PIN,
uint16_t BAUDRATE,
uint8_t SERCOMNUM) {

	module->TX				= TX_PIN;
	module->RX				= RX_PIN;
	module->BAUD			= BAUDRATE;
	module->SERCOMNUMBER	= SERCOMNUM;

	//SaLPinMode(module->TX,OUTPUT);
	//SaLPinMode(module->RX,INPUT);


}