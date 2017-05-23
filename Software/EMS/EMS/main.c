/*			 ________  ___ _____ 
//			|  ___|  \/  |/  ___|
//			| |__ | .  . |\ `--. 
//			|  __|| |\/| | `--. \
//			| |___| |  | |/\__/ /
//			\____/\_|  |_/\____/ 
//
//	Main code for Environmental Monitoring System - 10.05.2017
*/

#include "header.h"               
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include "System/BME280/bme280_i2c.h"
#include "System/BME280/i2cmaster.h"

// UART
#include "UART/STDIO_UART.h"
#define BAUD 9600

volatile unsigned long timer;

int main(void)
{
	ioinit();			// Init. UART
	setup();			// Init. setup
	
	while(1)
	{
		if (sec == true)
		{
			sec = false;
			int gas = readGas();
			int limit = 600;
			if (gas>limit)
			{
				// Enable realtime datastream every second if set gas level exceeds limit
				gatherData();
				transmit(dataStream);
			}
		}
	}
}

void setup(void)
{
	// Analog multiplexer
	DDRD |= (S0 | S1 | S2);	// PD3, PD4, PD5 as OUTPUT
	enableChannel(CH0);		// Start at channel 0
	
	// Capacitor pins
	DDRC |= A0;				// A0 as OUTPUT
	DDRC |= A1;				// A1 as OUTPUT
	
	// 16-bit Timer1
	TCCR1A = (1 << WGM11);	// Set CTC Bit
	OCR1B = 156;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS10);	// No prescaler
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	sei();					// Enable interrupt
	
	_delay_ms(2000);		// 2 seconds before executing code
}

void gatherData(void)
{
	// ID
	uint8_t getID = eeprom_read_byte((uint8_t*)location);	// Get ID from EEPROM
	intToHex(getID);										// Transform it to HEX
	assignData(getID,hex,1);								// Assign to dataStream
	
	// GAS
	int getGas = readGas();
	intToHex(getGas);
	assignData(getGas,hex,2);
	
	// CELL1
	int getCell1 = readCell1();
	intToHex(getCell1);
	assignData(getCell1,hex,2);
	
	// CELL2
	int getCell2 = readCell1();
	intToHex(getCell2);
	assignData(getCell2,hex,2);
	
	// RAIN
	int getRain = readCapacitance();
	intToHex(getRain);
	assignData(getRain,hex,3);
	
	// LIGHT
	int getLight = readLight();
	intToHex(getLight);
	assignData(getLight,hex,4);
	
	// Read from BME280 (temperature, humidity, pressure)
	int32_t temp;
	uint32_t press;
	uint32_t hum;
	BME280_readout(&temp, &press, &hum);
	
	// TEMP
	intToHex(temp);
	assignData(temp,hex,5);
	
	// HUM
	intToHex(hum);
	assignData(hum,hex,6);
	
	// PRESS
	intToHex(press);
	assignData(press,hex,7);
	
	// SPD
	
	// DIR
	const char* windDir = windDirection();
		
}

void assignData(int dec, const char *hex, int place)
{
	switch(place)
	{
		case 1:		// ID
		if (dec<15)
		{
			dataStream[0] = 0;
			dataStream[1] = hex [0];
		}
		else
		{
			dataStream[0] = hex [0];
			dataStream[1] = hex [1];
		}
		break;
		case 2:		// GAS
		
		break;
		case 3:		// CELL1
		
		break;
		case 4:		// CELL2
		
		break;
		case 5:		// RAIN
		
		break;
		case 6:		// LIGHT
		
		break;
		case 7:		// TEMP
		
		break;
		case 8:		// HUM
		
		break;
		case 9:		// PRESS
		
		break;
		case 10:	// SPD
		
		break;
		case 11:	// DIR
		
		break;
	}
	
}

// Interrupt Service Routine (approx. every second)
ISR(TIMER1_COMPA_vect)
{
	++timer;
	
	if(timer > 15600)	
	{
		sec = true;
	}
}
