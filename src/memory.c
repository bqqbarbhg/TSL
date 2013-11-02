#include "memory.h"

extern void *(*tsl_malloc)(size_t size) = malloc;
extern void *(*tsl_realloc)(void *ptr, size_t size) = realloc;
extern void (*tsl_free)(void *ptr) = free;

void tsl_SetMemoryHooks(tsl_Malloc_t nMalloc, tsl_Realloc_t nRealloc, tsl_Free_t nFree)
{
	tsl_malloc = nMalloc ? nMalloc : malloc;
	tsl_realloc = nRealloc ? nRealloc : realloc;
	tsl_free = nFree ? nFree : free;
}