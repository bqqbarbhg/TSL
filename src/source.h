#ifndef TSL_SOURCE_H
#define TSL_SOURCE_H

#include <stdio.h>

// Character source
typedef struct TSLSource_s
{
	// Buffer that holds character data
	char *buf;

	// Current character to read
	char *ptr;

	// Pointer to the character following the final character
	char *end;

	// Function that reads characters to `buf`
	// `state` can hold custom state for the method
	// Should return the end of the read input
	char *(*get)(void* state, char *buf);

	// Frees the source
	void(*free)(struct TSLSource_s *);

	// Custom state
	void *state;
} TSLSource;

static char tslSource_peek(TSLSource *src)
{
	if (src->ptr == src->end)
	{
		src->end = src->get(src->state, src->buf);
		src->ptr = src->buf;
	}
	return *src->ptr;
}

static char tslSource_get(TSLSource *src)
{
	char c = tslSource_peek(src);
	src->ptr++;
	return c;
}

static char tslSource_next(TSLSource *src)
{
	if (src->ptr != src->end)
		src->ptr++;
	return tslSource_peek(src);
}

// Reads a single utf-8 character to `buf`
// `buf` must be at least 4 bytes long
// Does not add a null byte after the character
// Returns: pointer to the end of the character (`buf` if invalid character)
char *tslSource_getUtf8Char(TSLSource *src, char *buf);

// Creates a TSLSource that reads from `buf`
// `buf` must be a 0-terminated string that is valid as long as `src` is
// Returns: 0 on success, non-0 on error
int tslSource_cStringRef(TSLSource *src, const char* buf);

// Cretes a TSLSource to read buffered from file poitned at `filename`
// Returns: 0 on success, non-0 on error
int tslSource_fileOpen(TSLSource *src, const char* filename);

// Cretes a TSLSource to read buffered from `file`
// Overwrites `src`, takes ownage of `file` (frees it when `tslSource_free` is called)
// Returns: 0 on success, non-0 on error
int tslSource_filePtr(TSLSource *src, FILE* file);

// Frees the TSLSource object (calls `src->free`)
void tslSource_free(TSLSource *src);

#endif
