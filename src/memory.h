#ifndef TSL_MEMORY_H
#define TSL_MEMORY_H

#include <stdlib.h>

extern void *(*tsl_malloc)(size_t size);
extern void *(*tsl_realloc)(void* ptr, size_t size);
extern void (*tsl_free)(void* ptr);

#endif
