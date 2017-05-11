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

volatile unsigned long timer;



// UART
#define BUAD	9600
#define BRC		((F_CPU/16/BUAD) - 1)
#define TX_BUFFER_SIZE	128
char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;
void appendSerial(char c);
void serialWrite(char  c[]);

// ADC
uint16_t ReadADC(uint8_t adcx);

#define A0 0x01				// PC0
#define A1 0x02				// PC1


int main(void)
{
	setup();
	
	serialWrite("Read capacitance\n\r");
	while(1)
	{
		//
	}
}

void setup(void)
{
	DDRB = 0x20;	
	DDRC |= A0;				// A0 as OUTPUT
	DDRC &= ~(A1);			// A1 as INPUT
	PORTC |= A0;			// Internal pull-up resistor on A0
	
	// 16-bit Timer1
	TCCR1A = (1 << WGM11);	//Set CTC Bit
	OCR1A = 156;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS10) | (1 << CS12);	//No pre-scaler used
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	
	sei();					// Enable interrupt
	
}


float readCapacitance(void)
{
	float capacitance;
	int adc_result;
	int pullup = 34.8;		// 34.8k ohm (might need calibration)
	unsigned long t;
	int i;
	
	do 
	{
		int i = PINC1;		// Check if A1 is high
		t++;
		_delay_us(1);
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

ISR(USART_TX_vect)
{
	if(serialReadPos != serialWritePos)
	{
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos++;
		
		if(serialReadPos >= TX_BUFFER_SIZE)
		{
			serialReadPos++;
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	++timer;
	
	if(timer > 50)		
	{
		// check gas approx. every 1.5s
	}
}

