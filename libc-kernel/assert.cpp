#include <assert.h>
#include <stdlib.h>
#include <console.h>

extern "C" assert(int expr)
{
	if (expr == 0)
	{
		VideoRamConsole << "LEL assertion failed BL MATE~!";
		abort();
	}
}