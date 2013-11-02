#ifndef TSL_MEMORY_H
#define TSL_MEMORY_H

#include <stdlib.h>

extern void *(*tsl_malloc)(size_t size);
extern void *(*tsl_realloc)(void *ptr, size_t size);
extern void (*tsl_free)(void *ptr);

typedef void *(*tsl_Malloc_t)(size_t);
typedef void *(*tsl_Realloc_t)(void *ptr, size_t);
typedef void (*tsl_Free_t)(void *ptr);

void tsl_SetMemoryHooks(tsl_Malloc_t nMalloc, tsl_Realloc_t nRealloc, tsl_Free_t nFree);

#endif
