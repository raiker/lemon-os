#include <stdlib.h>

extern "C" int atoi(const char* str)
{
	//Convert from a string to an int
	//No validation is performed. Will only work for proper number.
	char* strptr = str;
	
	//strip whitespace
	while (*strptr == ' ' || *strptr == '\t')
		strptr++;
	
	//neg or not?
	int sign;
	if (*strptr == '-')
	{
		sign = -1;
		strptr++;
	}
	else
		sign = 1;
	
	int base = 0;
	//next comes the base.
	//If there is only one character left, it is decimal
	//If not, and the next char is 0, it's octal
	//If the next two chars are 0x, it's hex
	//Else, it's decimal
	if (*strptr == 0) //null wtf?
		return 0; //throw some shit more like it
	if (*(strptr+1) == 0) //one-character decimal
		base = 10;
	else if (*(strptr) == '0' && *(strptr+1) == 'x')
		base = 16;
	else if (*(strptr) == '0')
		base = 8;
	
	if (base == 0) return 0; //and bitch angrily
	
	