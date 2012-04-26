//stdint.h hackjob because the system one depends on a whole bunch of crap

#ifndef __STDINT_H
#define __STDINT_H

typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;

typedef uint32_t uintptr_t

//And because i can think of nowhere better to put this
#define NULL 0

#endif
