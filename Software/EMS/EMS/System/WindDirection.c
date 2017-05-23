#include "../header.h"
#include <avr/io.h>
#include <string.h>


char* windDirection(void)
{
	enableChannel(CH3);
	int i = ReadADC(7);
	char *winDir= malloc (sizeof (char) * 2);
	
	if (i > 230 && i < 250)
	{
		strcpy(winDir,"N");
	}
	if (i > 110 && i < 150)
	{
		strcpy(winDir,"NE");
	}
	if (i > 10 && i < 140)
	{
		strcpy(winDir,"E");
	}
	if (i > 840 && i < 880)
	{
		strcpy(winDir,"SE");
	}
	if (i > 710 && i < 750)
	{
		strcpy(winDir,"S");
	}
	if (i > 590 && i < 630)
	{
		strcpy(winDir,"SW");
	}
	if (i > 490 && i < 530)
	{
		strcpy(winDir,"W");
	}
	if (i > 340 && i < 380)
	{
		strcpy(winDir,"NW");
	}
	return(char *)winDir;
	
}