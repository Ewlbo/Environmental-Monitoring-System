#include "../header.h"
#include <string.h>

void intToHex(int var)
{
	memset(hex, 0, 5);
	int i;
	
	if (var<=15)
	{
		if (var<=9)
		{
			itoa(var,hex,10);		// Int to string
		}
		else
		{
			switch(var)
			{
				case 10:
				strcpy(hex,"A");
				break;
				case 11:
				strcpy(hex,"B");
				break;
				case 12:
				strcpy(hex,"C");
				break;
				case 13:
				strcpy(hex,"D");
				break;
				case 14:
				strcpy(hex,"E");
				break;
				case 15:
				strcpy(hex,"F");
			}
		} 
	}
	else
	{
		long int quotient;
		int temp;
		i = 0;
		quotient = var;
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
			hex[i++]= temp;
			quotient = quotient / 16;
		}
	}
}