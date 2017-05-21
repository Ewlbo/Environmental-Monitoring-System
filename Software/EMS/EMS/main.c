//			 ________  ___ _____ 
//			|  ___|  \/  |/  ___|
//			| |__ | .  . |\ `--. 
//			|  __|| |\/| | `--. \
//			| |___| |  | |/\__/ /
//			\____/\_|  |_/\____/ 
//
//	Main code for Environmental Monitoring System - 10.05.2017
//


#include "header.h"               
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>



nRF24L01 *setup_rf(void);
void transmit(char *message);
void receive(void);
void process_message(char *message);
void setup(void);

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




int main(void)
{
	setup();
	

	
}

void setup(void)
{
	// Capacitor pins
	DDRC |= A0;				// A0 as OUTPUT
	DDRC |= A1;				// A1 as OUTPUT
	
	// 16-bit Timer1
	TCCR1A = (1 << WGM11);	// Set CTC Bit
	OCR1B = 156;
	TIMSK1 = (1 << OCIE1A);
	TCCR1B = (1 << CS10);	// No prescaler
	
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



void transmit(char *message)
{
	char text[29];									// Placeholder
	strcpy(text, message);							// Copy string from *message to placeholder
	int messageLength = strlen(text);				// Get the length

	if (messageLength>29)							// If string exceeds length (nRF24L01 max packet size = 32 bytes)
	{
		memset(text, 0, sizeof(text));				// Clear string
		strcpy(text, "Error: string too long");
		messageLength = strlen(text);
	}

	uint8_t to_address[5] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 };
	nRF24L01 *rf = setup_rf();
	nRF24L01Message msg;
	nRF24L01_flush_transmit_message(rf);
	memcpy(msg.data, text, messageLength);
	msg.length = strlen((char *)msg.data) + 1;
	nRF24L01_transmit(rf, to_address, &msg);
}

void receive(void)
{
	uint8_t address[5] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 };
	nRF24L01 *rf =setup_rf();
	nRF24L01_listen(rf,0, address);
	uint8_t addr[5];
	nRF24L01_read_register(rf, CONFIG, addr,1);
	while (nRF24L01_data_received(rf))
	{
		nRF24L01Message msg;
		nRF24L01_read_received_data(rf, &msg);
		process_message((char *)msg.data);
	}
	nRF24L01_listen(rf, 0, address);
}

void process_message(char *message)
{
	if (strcmp(message, "Message") == 0)
	{
		// Do something
	}
}

nRF24L01 *setup_rf(void) {
	nRF24L01 *rf = nRF24L01_init();
	rf->ss.port = &PORTB;
	rf->ss.pin = PB1;
	rf->ce.port = &PORTB;
	rf->ce.pin = PB2;
	rf->sck.port = &PORTB;
	rf->sck.pin = PB5;
	rf->mosi.port = &PORTB;
	rf->mosi.pin = PB3;
	rf->miso.port = &PORTB;
	rf->miso.pin = PB4;
	nRF24L01_begin(rf);
	return rf;
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
	
	if(timer > 15600)	// Approx. every second
	{
		// check gas approx. every 1.5s
	}
}
