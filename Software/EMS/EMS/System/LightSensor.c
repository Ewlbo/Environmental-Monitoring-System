#include "../header.h"
#include <avr/io.h>

int readLight(void)
{
	enableChannel(CH0);
	int light = ReadADC(7);
	return (light);
}