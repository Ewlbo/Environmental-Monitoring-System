#include "../header.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int readWindSpeed(void)
{
	int measuring = true;
	int ms = 0;
	unsigned long measureTime = 10000; // 10 Seconds
	int offState = 800;		// ADC value when IR LED is not blocked
	int revolution = 0;
	
	enableChannel(CH0);		// Read from analog multiplexer channel 4
	//PORTD |= 0x80;			// Enable IR LED
	
	while(measuring)
	{
		int IR = ReadADC(0);
		if (IR>offState)
		{
			revolution++;
		}
		ms++;
		_delay_ms(1);
		if (ms>=measureTime)
		{
			measuring = false;
			//PORTD &= ~(0x80);	// Disable IR LED
			break;
		}
	}
	
	return (revolution);
}