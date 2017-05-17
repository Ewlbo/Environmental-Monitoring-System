/*
 * wind_speed.c
 *
 * Created: 17-05-2017 09:50:09
 * Author : Soren
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

// ADC

uint16_t ReadADC(uint8_t adcx);

// UART

#define BUAD	9600
#define BRC		((F_CPU/16/BUAD) - 1)
#define TX_BUFFER_SIZE	128
char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;
void appendSerial(char c);
void serialWrite(char  c[]);

int main(void)
{
	// UART
	UBRR0H = (BRC >> 8);
	UBRR0L =  BRC;
	UCSR0B = (1 << TXEN0)  | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// ADC
	ADMUX=(1<<REFS0);									// For Aref = AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Pre-scaler div factor =128
	
	sei();
	//init ADC
	
    float voltage = 4.7; 
	int adc_result;
	int Rev;
	
   for (int i = 0; i<10;i++)
   {
	   if (adc_result > voltage)
	   {
		   Rev++;
	   }
	   _delay_ms(1000);
   }
		
    
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