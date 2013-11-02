#ifndef TSL_TOKEN_H
#define TSL_TOKEN_H

#include <stdlib.h>
#include "source.h"

typedef enum TSLTokenType_e
{
	TSLTOK_IDENTIFIER,
	TSLTOK_END,
} TSLTokenType;

typedef struct TSLToken_s
{
	TSLTokenType type;
	union {
		const char *string;
		long integer;
		double floating;
	} val;
} TSLToken;

typedef struct TSLTokenStream_s
{
	TSLSource source;

	char *errbuf;
	size_t errbuf_sz;
} TSLTokenStream;

// Read next token from `src`
// Advances `src`, overwrites `dst`
// Returns 0 on success, non-0 on error
int tslToken_get(TSLTokenStream *stream, TSLToken *dst);

#endif
