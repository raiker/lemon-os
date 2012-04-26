#include <stdlib.h>

extern "C" void* calloc(size_t num, size_t size)
{
	void* mem = kmalloc(num*size);
	for (int i = 0; i < num*size; i++)
		*(mem+i) = 0;
	return mem;
}