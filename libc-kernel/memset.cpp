#include <stdlib.h>

extern "C" void* memset(void* ptr, int value, size_t num)
{
	for (int i = 0; i < num; i++)
		*(ptr+i) = (unsigned char)value;
	return ptr;
}