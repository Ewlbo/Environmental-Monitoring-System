//	______ _____  ___ ______      _____   ___  ______  ___  _____ _____ _____ ___   _   _ _____  _____
//	| ___ \  ___|/ _ \|  _  \    /  __ \ / _ \ | ___ \/ _ \/  __ \_   _|_   _/ _ \ | \ | /  __ \|  ___|
//	| |_/ / |__ / /_\ \ | | |    | /  \// /_\ \| |_/ / /_\ \ /  \/ | |   | |/ /_\ \|  \| | /  \/| |__
//	|    /|  __||  _  | | | |    | |    |  _  ||  __/|  _  | |     | |   | ||  _  || . ` | |    |  __|
//	| |\ \| |___| | | | |/ /     | \__/\| | | || |   | | | | \__/\_| |_  | || | | || |\  | \__/\| |___
//	\_| \_\____/\_| |_/___/       \____/\_| |_/\_|   \_| |_/\____/\___/  \_/\_| |_/\_| \_/\____/\____/
//
//	Read capacitance from Telecontrolli capacitive rain sensor - for EMS 12.05.2017
//


#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

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
	
	char string[8];
	serialWrite("Read capacitance\n\r");
	
	while(1)
	{
		float capacitance = readCapacitance();
		itoa(capacitance, string, 10);		// Convert float to string
		serialWrite(string);
		serialWrite("\n\r");
		_delay_ms(1000);
	}
}

void setup(void)
{
	// Capacitor pins
	DDRC |= A0;				// A0 as OUTPUT
	DDRC |= A1;				// A1 as OUTPUT
	
	// 16-bit Timer1
	TCCR1A = (1 << WGM11);	//Set CTC Bit
	OCR1A = 156;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS10) | (1 << CS12);	//No pre-scaler used
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	// UART
	UBRR0H = (BRC >> 8);
	UBRR0L =  BRC;
	UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	sei();					// Enable interrupt
	
}

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

