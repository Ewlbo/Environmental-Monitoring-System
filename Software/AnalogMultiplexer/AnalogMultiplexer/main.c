#include <avr/io.h>

void enableChannel(int ch);

#define S0 0x08			// PD3
#define S1 0x10			// PD4
#define S2 0x20			// PD5
#define LIGHT_S 0
#define PHOTOCELL_1 1
#define PHOTOCELL_2 2
#define	WIND_DIR 3
#define WIND_SPD 4

int main(void)
{
	DDRD |= (S0 | S1 | S2);			// PD3, PD4, PD5 as OUTPUT
	
	enableChannel(LIGHT_S);
}


void enableChannel(int ch)
{
	switch(ch)
	{
		case 0:
		PORTD &= ~(S0 | S1 | S2);	// All LOW (check flowchart for select pin states)
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


