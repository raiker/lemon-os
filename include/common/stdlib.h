#include <kmalloc.h>

#ifndef __STDLIB_H
#define __STDLIB_H

int atoi(const char* str);
char* itoa(int val, char* buff, int base);

void abort();

#define malloc kmalloc
#define free kfree
void* calloc(size_t num, size_t size);

void* memset(void* ptr, int value, size_t num);

#endif