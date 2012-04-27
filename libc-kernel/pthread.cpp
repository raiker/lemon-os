#include <pthread.h>

namespace { void* threadDataTable[64]; int freeEntry = 0;}
extern "C" {
int pthread_key_create(pthread_key_t* key, void (*)(void*)) {
	assert(freeEntry < 64);
 
	*key = freeEntry;
	freeEntry++;
	return 0;
}
 
int pthread_once(pthread_once_t* control, void (*init)(void)) {
	if (*control == 0) {
		(*init)();
		*control = 1;
	}
	return 0;
}
 
void* pthread_getspecific(pthread_key_t key) {
	return threadDataTable[key];
}
 
int pthread_setspecific(pthread_key_t key, const void* data) {
	threadDataTable[key] = (void*)data;
	return 0;
}
 
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t*) {
	*mutex = 0;
	return 0;
}
 
int pthread_mutex_lock(pthread_mutex_t* mutex) {
	assert(*mutex == 0);
	*mutex = 1;
	return 0;
}
 
int pthread_mutex_unlock(pthread_mutex_t* mutex) {
	assert(*mutex != 0);
	*mutex = 0;
	return 0;
}
 
int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*) {
	return 0;
}
 
int pthread_cond_signal(pthread_cond_t*) {
	return 0;
}

"