#include "../header.h"
#include <avr/io.h>

int readLight(void)
{
	enableChannel(CH2);
	int light = ReadADC(0);
	return (light);
}