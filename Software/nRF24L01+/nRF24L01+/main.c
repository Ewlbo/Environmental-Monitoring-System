//		      ____________ _____   ___  _     _____  __       
//	              | ___ \  ___/ __  \ /   || |   |  _  |/  |  _   
//		 _ __ | |_/ / |_  `' / /'/ /| || |   | |/' |`| |_| |_ 
//		| '_ \|    /|  _|   / / / /_| || |   |  /| | | |_   _|
//		| | | | |\ \| |   ./ /__\___  || |___\ |_/ /_| |_|_|  
//		|_| |_\_| \_\_|   \_____/   |_/\_____/\___/ \___/     
//                                                      
//      Nordic Semiconductor nRF24L01+ RF module for EMS 12.05.2017
//		Copyright (c) 2014 Antoine Leclair
//
                                        

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"

nRF24L01 *setup_rf(void);
void transmit(void);
void receive(void);
void process_message(char *message);


int main(void)
{

}

void transmit(void)
{
	uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
	nRF24L01 *rf = setup_rf();
	nRF24L01Message msg;
	memcpy(msg.data, "Transmission started", 20);
	msg.length = strlen((char *)msg.data) + 1;
	nRF24L01_transmit(rf, to_address, &msg);
	nRF24L01_flush_transmit_message(rf);
}

void receive(void)
{
	uint8_t address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
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
	// Interrupt on falling edge of INT0 (PD2)
	EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	nRF24L01_begin(rf);
	return rf;
}
