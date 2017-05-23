//	______ _____  ___ ______      _____   ___  ______  ___  _____ _____ _____ ___   _   _ _____  _____
//	| ___ \  ___|/ _ \|  _  \    /  __ \ / _ \ | ___ \/ _ \/  __ \_   _|_   _/ _ \ | \ | /  __ \|  ___|
//	| |_/ / |__ / /_\ \ | | |    | /  \// /_\ \| |_/ / /_\ \ /  \/ | |   | |/ /_\ \|  \| | /  \/| |__
//	|    /|  __||  _  | | | |    | |    |  _  ||  __/|  _  | |     | |   | ||  _  || . ` | |    |  __|
//	| |\ \| |___| | | | |/ /     | \__/\| | | || |   | | | | \__/\_| |_  | || | | || |\  | \__/\| |___
//	\_| \_\____/\_| |_/___/       \____/\_| |_/\_|   \_| |_/\____/\___/  \_/\_| |_/\_| \_/\____/\____/
//
//	Read capacitance from Telecontrolli capacitive rain sensor - for EMS 12.05.2017
//

#include <avr/io.h>
#include "../header.h"

float readCapacitance(void)
{
	float capacitance;
	int adc_result;
	float IC = 24.48;			// Internal capacitance (error)
	
	DDRC &= ~(A0);				// A0 as INPUT
	PORTC |= A1;				// A1 HIGH (charge)
	adc_result = ReadADC(0);	// Read ADC from A0
	PORTC &= ~(A1);				// A1 LOW (discharge)
	DDRC |= A0;					// A0 as OUTPUT (to make sure it discharges)
	
	capacitance = (adc_result*IC)/(1023-adc_result);
	
	return(capacitance);
}

