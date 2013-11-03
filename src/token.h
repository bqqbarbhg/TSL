#ifndef TSL_TOKEN_H
#define TSL_TOKEN_H

#include <stdlib.h>
#include "source.h"

typedef enum TSLTokenErrorType_e
{
	TSLLEXERR_UNEXPECTED_CHARACTER,
	TSLLEXERR_INVALID_HEX_DIGIT,
	TSLLEXERR_INVALID_UNCIODE_ESCAPE,
	TSLLEXERR_UNKNOWN_ESCAPE_SEQUENCE,
} TSLTokenErrorType;

typedef enum TSLTokenType_e
{
	TSLTOK_STRING,
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

typedef struct TSLLexer_s
{
	TSLSource source;

	char *tmpStrBuf;
	size_t tmpStrBufSize;

	TSLTokenErrorType errType;
	char *errbuf;
	size_t errbuf_sz;
	size_t col, line;
} TSLLexer;


// Initializes the lexer
// `source` must be valid before calling this
void tslLexer_init(TSLLexer *lex);

// Read next token from `src`
// Advances `src`, overwrites `dst`
// Returns 0 on success, non-0 on error
int tslLexer_scan(TSLLexer *lex, TSLToken *dst);

// Frees the lexer
void tslLexer_free(TSLLexer *lex);

#endif
