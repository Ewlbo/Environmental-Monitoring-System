#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>

unsigned long micro(void);
void setup(void);
float readCapacitance(void);

// ADC
uint16_t ReadADC(uint8_t adcx);

#define A0 0x01				// PC0
#define A1 0x02				// PC1


int main(void)
{
	setup();
}

void setup(void)
{
	DDRC |= A0;			// A0 as OUTPUT
	DDRC &= ~(A1);		// A1 as INPUT
	PORTC |= A0;		// Internal pull-up resistor on A0
	
	// For interrupt every 1 microsecond
	TCCR0A = (1 << WGM01);			// Set CTC Bit
	OCR0A = 16;						// 16 ticks for 1 microsecond
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1 << CS00);			// No pre-scaler used
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	
	sei();												// Enable interrupt
	
}


float readCapacitance(void)
{
	float capacitance;
	int adc_result;
	int pullup = 34.8;				// 34.8k ohm (might need calibration)
	unsigned long t;
	unsigned long t1 = micro();
	int i;
	
	do 
	{
		int i = PINC1;				// Check if A1 is high
		unsigned long t2 = micro();
		t = t2 - t1;
	} while (i<1);
	
	PORTC &= ~(A1);			// Disable pull-up
	adc_result = ReadADC(1);
	PORTC |= A0;			// A0 HIGH
	_delay_ms(1);
	DDRC |= A1;				// A1 as OUTPUT
	PORTC &= ~(A1);			// A1 LOW
	PORTC &= ~(A0);			// A0 LOW
	float x = adc_result/1023;
	float l = log(1.0 - x);
	capacitance = -(t/pullup)/l;
	return(capacitance);
	
}

unsigned long micro(void)
{
	unsigned long microseconds;
	microseconds++;
	return(microseconds);
}



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


ISR(TIMER0_COMPA_vect)
{
	unsigned long timer;
	timer++;
	if (timer == 4200000000)		// Exactly 70 minutes
	{
		timer = 0;
	}
	micro();
}

