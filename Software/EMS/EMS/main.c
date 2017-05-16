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
#include <time.h>

nRF24L01 *setup_rf(void);
void transmit(char *message, int var);
void receive(void);
void process_message(char *message);
void setup(void);

int main(void)
{
	int temp = 26;
	transmit("Temperature ",temp);
}

void transmit(char *message, int var)
{
	char variable[8];								// Placeholder
	itoa(var, variable, 10);						// Convert integer to string
	char text[30];									// Placeholder
	strcpy(text, message);							// Copy string from pointer *message
	strcat(text, variable);							// Combine *message and variable
	int messageLength = strlen(text);				// Get the length
	
	if (messageLength>30)							// If string exceeds length (nRF24L01 max packet size = 32 bytes)
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