#include "memory.h"

extern void *(*tsl_malloc)(size_t size) = malloc;
extern void *(*tsl_realloc)(void *ptr, size_t size) = realloc;
extern void (*tsl_free)(void* ptr) = free;