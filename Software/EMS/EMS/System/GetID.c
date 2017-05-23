#include <avr/eeprom.h>
#include "../header.h"
#include <string.h>

char *getID(void)
{
	char *IDarray= malloc(1000);
	uint8_t ID = eeprom_read_byte ((uint8_t*)location);
	long int quotient;
	int i=1;
	int temp;
	memset(IDarray, 0, sizeof(*IDarray));	// Clear string
	quotient = ID;
	while(quotient!=0)
	{
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
		{
			temp =temp + 48;
		}
		else
		{
			temp = temp + 55;
		}
		IDarray[i--]= temp;
		quotient = quotient / 16;
	}
	
	return(char *)IDarray;
	free(IDarray);
}