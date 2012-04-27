#include <console.h>

#ifndef __STDIO_H
#define __STDIO_H

#define stderr 2

inline void fprintf(int fh, const char* str) { VideoRamConsole << str; }


#endif
