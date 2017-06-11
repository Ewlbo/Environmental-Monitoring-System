// ISR is set to approx. 1s on TIMER1
// UART is set on TIMER0
// 
// Typical data stream in HEX:
// 4F06636C35513A21D-0F613E914NW
// Data stream   4F 066 36C   355   13A      21D   -0F     61         3E9   14          NW  == 29 long in hex
// Type          ID GAS CELL1 CELL2 RAIN     LIGHT TEMP    HUM        PRESS SPD         DIR
// Data type        ADC ADC   ADC   CAPACITY ADC   DEGREES PERCENTAGE hPa   REVOLUTIONS
// Place #       1  2   3     4     5        6     7       8          9     10          11

#ifndef HEADER_H_
#define HEADER_H_

#define F_CPU 8000000UL

#include "RF/nrf24l01.h"
#include "RF/nrf24l01-mnemonics.h"

#define false 0
#define true 1


// Forward declaration
void enableChannel(int ch);
nRF24L01 *setup_rf(void);
void transmit(char *message);
void receive(void);
void process_message(char *message);
void setup(void);
void assignData(int dec, const char *hex, int place);
void intToHex(int var);
void gatherData(void);
int readGas(void);
int readCell1(void);
int readCell2(void);
int readLight(void);
float readCapacitance(void);
void windDirection(void);
int readWindSpeed(void);
void realTime(void);

// Data stream to be sent with RF module
char dataStream[32];
char hex[5];

// EPPROM 
#define location 42			// Location of EEPROM memory - EMS default = 42

// ADC
uint16_t ReadADC(uint8_t adcx);

// Rain sensor
#define A0 0x02				// PC1 (capacitive rain sensor pin 1)
#define A1 0x04				// PC2 (capacitive rain sensor pin 2)

// Analog multiplexer
#define S0 0x04			// Select pin S0 on PD2
#define S1 0x08			// Select pin S1 on PD3
#define CH0 0			// Light sensor
#define CH1 1			// Photocell 1
#define CH2 2			// Photocell 2
#define	CH3 3			// Wind direction




#endif /* HEADER_H_ */