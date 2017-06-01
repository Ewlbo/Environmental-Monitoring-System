#include "../header.h"
#include <avr/io.h>

int readGas(void)
{
	int gasADC = ReadADC(3);
	return (gasADC);
}