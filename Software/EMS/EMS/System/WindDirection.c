#include "../header.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

int wDir = 11;

void windDirection(void)
{
	
	enableChannel(CH1);
	
	int i = ReadADC(0);
	if (i<10)
	{
		i = wDir;
	}
	else
	{
		wDir = i;
	}
	
	if (i > 230 && i < 250)
	{
		dataStream[27] = 78;
		dataStream[28] = 0;
	}
	if (i > 110 && i < 150)
	{
		dataStream[27] = 78;
		dataStream[28] = 69;
	}
	if (i > 10 && i < 140)
	{
		dataStream[27] = 69;
		dataStream[28] = 0;
	}
	if (i > 840 && i < 880)
	{
		dataStream[27] = 83;
		dataStream[28] = 69;
	}
	if (i > 710 && i < 750)
	{
		dataStream[27] = 83;
		dataStream[28] = 0;
	}
	if (i > 590 && i < 630)
	{
		dataStream[27] = 83;
		dataStream[28] = 87;
	}
	if (i > 490 && i < 530)
	{
		dataStream[27] = 87;
		dataStream[28] = 0;
	}
	if (i > 340 && i < 380)
	{
		dataStream[27] = 78;
		dataStream[28] = 87;
	}	
}