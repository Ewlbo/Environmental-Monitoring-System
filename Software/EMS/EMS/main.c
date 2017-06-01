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
//#include "System/BME280/bme280_i2c.h"
//#include "System/BME280/i2cmaster.h"
#include "System/BMP180/bmp085.h"
#include "System/dht/dht.h"

// UART
#include "UART/STDIO_UART.h"
#define BAUD 9600

volatile unsigned long timer;
int sec = 0;
int alert = false;

int main(void)
{
	ioinit();			// Init. UART
	setup();			// Init. setup
	
	while(true)
	{
		// Do nothing
	}
}

void setup(void)
{
	_delay_ms(500);
	printf("EMS is running, no errors found\r");
	transmit("EMS is running ###############");

	// Analog multiplexer
	DDRD |= (S0 | S1 | S2);	// PD3, PD4, PD5 as OUTPUT
	enableChannel(CH0);		// Start at channel 0
	
	// Capacitor pins
	DDRC |= A0;				// A0 as OUTPUT
	DDRC |= A1;				// A1 as OUTPUT
	
	// IR LED toggle for wind speed measurement
	//DDRD |= 0x80;			// PD7 as OUTPUT
	//PORTD &= ~(0x80);		// PD7 LOW
	
	// 16-bit Timer1
	TCCR1A = (1 << WGM11);	// Set CTC Bit
	OCR1B = 156;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS10);	// No prescaler
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	sei();					// Enable interrupt
	_delay_ms(10);
	if (alert == true)
	{
		printf("Alert detected \r");
	}
	bmp085_init();
}

void gatherData(void)
{
	printf("Function gatherData(); called\r");
	// Fake data incoming:
	//int getGas = 500;
	int getCell1 = 823;
	int getCell2 = 800;
	//int getRain = 250;
	//int getLight = 150;
	//int temp = 24;
	//int hum = 55;
	//int press = 1005;
	//int getWindSpeed = 14;
	int8_t temperature = 0;
	int8_t hum = 0;
	
	// ID
	uint8_t getID = eeprom_read_byte((uint8_t*)location);	// Get ID from EEPROM
	intToHex(getID);										// Transform it to HEX
	assignData(getID,hex,1);								// Assign to dataStream
	
	// GAS SENSOR
	int getGas = readGas();
	intToHex(getGas);
	assignData(getGas,hex,2);
	
	// CELL1
	//int getCell1 = readCell1();
	intToHex(getCell1);
	assignData(getCell1,hex,3);
	
	// CELL2
	//int getCell2 = readCell1();
	intToHex(getCell2);
	assignData(getCell2,hex,4);
	
	// RAIN SENSOR
	int getRain = readCapacitance();
	intToHex(getRain);
	assignData(getRain,hex,5);
	
	// LIGHT SENSOR
	int getLight = readLight();
	intToHex(getLight);
	assignData(getLight,hex,6);
	
	// Read from BME280 (temperature, humidity, pressure)
	//int32_t temp;
	//uint32_t press;
	//uint32_t hum;
	//BME280_readout(&temp, &press, &hum);
	
	// TEMPERATURE
	int temp = bmp085_gettemperature();
	intToHex(temp);
	assignData(temp,hex,7);
	
	// HUMIDITY
	dht_gettemperaturehumidity(&temperature, &hum);
	intToHex(hum);
	assignData(hum,hex,8);
	
	// PRESSURE
	long press = bmp085_getpressure();
	int pressf = press/100;
	intToHex(pressf);
	assignData(pressf,hex,9);
	
	// WIND SPEED
	int getWindSpeed = readWindSpeed();
	intToHex(getWindSpeed);
	assignData(getWindSpeed,hex,10);
	
	// WIND DIRECTION
	windDirection();

	printf("Datastream: %s\r",dataStream);
}

void realTime(void)
{
	alert = true;
	printf("Realtime data enabled\r");
	gatherData();
	transmit(dataStream);
}

void getAlert(int state)
{
	switch(state)
	{
		case 0:
		alert = false;
		break;
		case 1:
		alert = true;
		printf("Alert detected \r");
		break;
	}
}

void assignData(int dec, const char *hex, int place)
{
	switch(place)
	{
		case 1:		// ID
		if (dec<=15)
		{
			dataStream[0] = 48;			// 0
			dataStream[1] = hex [0];
		}
		else
		{
			dataStream[0] = hex [1];
			dataStream[1] = hex [0];
		}
		break;
		case 2:		// GAS
		if (dec<=15)
		{
			dataStream[2] = 48;
			dataStream[3] = 48;
			dataStream[4] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[2] = 48;
			dataStream[3] = hex [1];
			dataStream[4] = hex [0];
		}
		else
		{
			dataStream[2] = hex [2];
			dataStream[3] = hex [1];
			dataStream[4] = hex [0];
		}
		break;
		case 3:		// CELL1
		if (dec<=15)
		{
			dataStream[5] = 48;
			dataStream[6] = 48;
			dataStream[7] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[5] = 48;
			dataStream[6] = hex [1];
			dataStream[7] = hex [0];
		}
		else
		{
			dataStream[5] = hex [2];
			dataStream[6] = hex [1];
			dataStream[7] = hex [0];
		}
		break;
		case 4:		// CELL2
		if (dec<=15)
		{
			dataStream[8] = 48;
			dataStream[9] = 48;
			dataStream[10] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[8] = 48;
			dataStream[9] = hex [1];
			dataStream[10] = hex [0];
		}
		else
		{
			dataStream[8] = hex [2];
			dataStream[9] = hex [1];
			dataStream[10] = hex [0];
		}
		break;
		case 5:		// RAIN
		if (dec<=15)
		{
			dataStream[11] = 48;
			dataStream[12] = 48;
			dataStream[13] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[11] = 48;
			dataStream[12] = hex [1];
			dataStream[13] = hex [0];
		}
		else
		{
			dataStream[11] = hex [2];
			dataStream[12] = hex [1];
			dataStream[13] = hex [0];
		}
		break;
		case 6:		// LIGHT
		if (dec<=15)
		{
			dataStream[14] = 48;
			dataStream[15] = 48;
			dataStream[16] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[14] = 48;
			dataStream[15] = hex [1];
			dataStream[16] = hex [0];
		}
		else
		{
			dataStream[14] = hex [2];
			dataStream[15] = hex [1];
			dataStream[16] = hex [0];
		}
		break;
		case 7:		// TEMP
		if (dec<0)
		{
			dataStream[17] = 45;		// -
			dec = dec * (-1);
		}
		else
		{
			dataStream[17] = 43;		// +
		}
		if (dec<=15)
		{
			dataStream[18] = 48;
			dataStream[19] = hex [0];
		}
		else
		{
			dataStream[18] = hex [1];
			dataStream[19] = hex [0];
		}
		break;
		case 8:		// HUM
		if (dec<=15)
		{
			dataStream[20] = 48;
			dataStream[21] = hex [0];
		}
		else
		{
			dataStream[20] = hex [1];
			dataStream[21] = hex [0];
		}
		break;
		case 9:		// PRESS
		if (dec<=15)
		{
			dataStream[22] = 48;
			dataStream[23] = 48;
			dataStream[24] = hex [0];
		}
		else if (dec>15 && dec<=255)
		{
			dataStream[22] = 48;
			dataStream[23] = hex [1];
			dataStream[24] = hex [0];
		}
		else
		{
			dataStream[22] = hex [2];
			dataStream[23] = hex [1];
			dataStream[24] = hex [0];
		}
		break;
		case 10:	// SPD
		if (dec<=15)
		{
			dataStream[25] = 48;
			dataStream[26] = hex [0];
		}
		else
		{
			dataStream[25] = hex [1];
			dataStream[26] = hex [0];
		}
		break;
	}
}

// Interrupt Service Routine (approx. every second)
ISR(TIMER1_COMPA_vect)
{
	++timer;
	
	if(timer > 15600)	
	{
		// For testing/presentation purposes enable realtime
		//realTime();
		
		timer = 0;
		int gas = readGas();
		int limit = 400;
		if (gas>limit)
		{
			// Enable realtime datastream if set gas level exceeds limit
			getAlert(true);
			printf("Gas exceeds limit. Enabling realtime data.\r");		
			realTime();
		}
		else
		{
			getAlert(false);
		}
		sec++;
	}
	if (sec>=3)
	{
		sec = 0;
		gatherData();
		transmit(dataStream);
	}
}
