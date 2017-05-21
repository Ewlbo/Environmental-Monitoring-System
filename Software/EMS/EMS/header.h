// ISR is set to approx. 1s on TIMER1
// UART is set on TIMER0 


#ifndef HEADER_H_
#define HEADER_H_

#define F_CPU 16000000UL

#include "RF/nrf24l01.h"
#include "RF/nrf24l01-mnemonics.h"
char stream[29];

#define A0 0x01				// PC0 (capacitive rain sensor pin 1)
#define A1 0x02				// PC1 (capacitive rain sensor pin 2)




#endif /* HEADER_H_ */