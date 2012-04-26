#include <stdlib.h>

extern "C" void abort()
{
	__asm("hlt");
}
