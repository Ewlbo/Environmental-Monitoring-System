Software of EMS
===============

Reading ADC
-------------------
Full code available in ADC folder

<code>
	// Select ADC Channel

	ADMUX	&=	0xf0;
	ADMUX	|=	0; ADC0

	// Start Single conversion

	ADCSRA|=(1<<ADSC);

	// Wait for conversion to complete

	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing 1 to it

	ADCSRA|=(1<<ADIF);

	return(ADC);

</code>

UART example
-------------------
