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

#define S0 0x08			// Select pin S0 on PD3
#define S1 0x10			// Select pin S1 on PD4
#define S2 0x20			// Select pin S2 on PD5

#define CH0 0			// Light sensor
#define CH1 1			// Photocell 1
#define CH2 2			// Photocell 2
#define	CH3 3			// Wind direction
#define CH4 4			// Wind speed

void enableChannel(int ch);

int main(void)
{
	DDRD |= (S0 | S1 | S2);			// PD3, PD4, PD5 as OUTPUT
	enableChannel(CH3);
}

void enableChannel(int ch)
{
	switch(ch)
	{
		case 0:
		PORTD &= ~(S0 | S1 | S2);	// All LOW (check flowchart and/or datasheet for select pin states)
		break;
		case 1:
		PORTD |= S0;
		PORTD &= ~(S1 | S2);
		break;
		case 2:
		PORTD |= S1;
		PORTD &= ~(S0 | S2);
		break;
		case 3:
		PORTD |= (S0 | S1);
		PORTD &= ~(S2);
		break;
		case 4:
		PORTD |= S2;
		PORTD &= ~(S0 | S1);
		break;
	}
}


