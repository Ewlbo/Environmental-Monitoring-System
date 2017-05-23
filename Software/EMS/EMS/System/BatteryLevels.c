#include "../header.h"
#include <avr/io.h>

int readCell1(void)
{
	int cell1 = ReadADC(2);
	return (cell1);
}

int readCell2(void)
{
	int cell2 = ReadADC(3);
	return (cell2);
}