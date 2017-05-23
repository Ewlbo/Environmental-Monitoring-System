#include <avr/io.h>
#include "../header.h"


uint16_t ReadADC(uint8_t adcx)
{
	// Select ADC Channel
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;

	
	// Start Single conversion
	ADCSRA|=(1<<ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing 1 to it
	ADCSRA|=(1<<ADIF);

	return(ADC);
}