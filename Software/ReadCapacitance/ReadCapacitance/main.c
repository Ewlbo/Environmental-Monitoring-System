#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

unsigned long milli(void);
void setup(void);
float readCapacitance(void);

// ADC
uint16_t ReadADC(uint8_t adcx);


// UART
#define F_CPU	16000000
#define BUAD	9600
#define BRC		((F_CPU/16/BUAD) - 1)
#define TX_BUFFER_SIZE	128
char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;
void appendSerial(char c);
void serialWrite(char  c[]);

#define A0 0x01				// PC0
#define A1 0x02				// PC1


int main(void)
{
	setup();
	serialWrite("Reading capacitance\n\r");
	int capacitance = readCapacitance();
}

void setup(void)
{
	DDRC |= A0;			// A0 as OUTPUT
	DDRC &= ~(A1);		// A1 as INPUT
	PINC |= A0;			// Internal pull-up resistor on A0
	
	// For interrupt every 1 microsecond
	TCCR0A = (1 << WGM01);			// Set CTC Bit
	OCR0A = 16;						// 16 ticks for 1 microsecond
	TIMSK0 = (1 << OCIE0A);
	
	sei();							// Enable interrupt
	
	TCCR0B = (1 << CS00);			// No pre-scaler used
	
	// UART
	UBRR0H = (BRC >> 8);
	UBRR0L =  BRC;
	
	UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128

	
}


float readCapacitance(void)
{
	float capacitance;
	
	return(capacitance);
	
}

unsigned long milli(void)
{
	unsigned long milliseconds;
	milliseconds++;
	return(milliseconds);
}



uint16_t ReadADC(uint8_t adcx)
{
	// Select ADC Channel
	ADMUX	&=	0xf0;
	ADMUX	|=	0;

	// Start Single conversion

	ADCSRA|=(1<<ADSC);

	// Wait for conversion to complete

	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing 1 to it

	ADCSRA|=(1<<ADIF);

	return(ADC);
}

void appendSerial(char c)
{
	serialBuffer[serialWritePos] = c;
	serialWritePos++;
	
	if(serialWritePos >= TX_BUFFER_SIZE)
	{
		serialWritePos = 0;
	}
}

void serialWrite(char c[])
{
	for(uint8_t i = 0; i < strlen(c); i++)
	{
		appendSerial(c[i]);
	}
	
	if(UCSR0A & (1 << UDRE0))
	{
		UDR0 = 0;
	}
}

ISR(TIMER0_COMPA_vect)
{
	unsigned long timer;
	timer++;
	if (timer == 4200000000)		// Exactly 70 minutes
	{
		timer = 0;
	}
	milli();
}