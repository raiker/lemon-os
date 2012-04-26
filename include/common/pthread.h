#include <stdint.h>

#ifndef __PTHREAD_H
#define __PTHREAD_H

typedef int32_t pthread_key_t;
typedef int32_t pthread_once_t;
typedef int32_t pthread_mutex_t;
typedef int32_t pthread_cond_t;
typedef int32_t pthread_mutexattr_t;

#define PTHREAD_ONCE_INIT 1
#define PTHREAD_MUTEX_INITIALIZER 1
#define PTHREAD_COND_INITIALIZER 1

int pthread_key_create(pthread_key_t* key, void (*)(void*));
int pthread_once(pthread_once_t* control, void (*init)(void));
void* pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void* data);
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t*);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*);
int pthread_cond_signal(pthread_cond_t*);
#endif