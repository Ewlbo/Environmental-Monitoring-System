//       _________  _   _ _____   ___ _____ _____ __  
//	    |___  /   || | | /  __ \ /   |  _  |  ___/  | 
//		   / / /| || |_| | /  \// /| | |/' |___ \`| | 
//		  / / /_| ||  _  | |   / /_| |  /| |   \ \| | 
//		./ /\___  || | | | \__/\___  \ |_/ /\__/ /| |_
//		\_/     |_/\_| |_/\____/   |_/\___/\____/\___/
//
//		74HC4051 analog multiplexer for EMS 13.05.2017
//
 

#include <avr/io.h>
#include "../header.h"


void enableChannel(int ch)
{
	PORTD &= ~(PD4);
	switch(ch)
	{
		case 0:
		PORTD &= ~(S0 | S1);	// All LOW (check flowchart and/or datasheet for select pin states)
		break;
		case 1:
		PORTD |= S0;
		PORTD &= ~(S1);
		break;
		case 2:
		PORTD |= S1;
		PORTD &= ~(S0);
		break;
		case 3:
		PORTD |= (S0 | S1);
		break;
	}
}


