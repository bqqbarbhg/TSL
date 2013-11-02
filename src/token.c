#include "token.h"
#include "source.h"
#include "util.h"
#include <stdio.h>

static char tslspeek(TSLLexer *lex)
{
	char c = tslSource_peek(&lex->source);
	while (c == '\r')
		c = tslSource_next(&lex->source);
	return c;
}

static char tslsnext(TSLLexer *lex)
{
	char c = tslSource_next(&lex->source);
	while (c == '\r')
		c = tslSource_next(&lex->source);

	if (c == '\n')
	{
		lex->line++;
		lex->col = 0;
	}
	else
	{
		lex->col++;
	}
		
	return c;
}

void tslLexer_init(TSLLexer *lex)
{
	lex->line = 1;
	lex->col = 0;
	if (tslspeek(lex) == '\n')
		lex->line++;
}

void tslLexer_free(TSLLexer *lex)
{
	tslSource_free(&lex->source);
}

int tslLexer_scan(TSLLexer *lex, TSLToken *dst)
{
	char c = tslspeek(lex);

	for (;;)
	{
		while (c == ' ' || c == '\t' || c == '\n')
			c = tslsnext(lex);
		if (c == '#')
		{
			// Skip line comment
			c = tslsnext(lex);
			while (c != '\n' && c != 0)
				c = tslsnext(lex);
			continue;
		}
		break;
	}

	if (c == 0)
	{
		dst->type = TSLTOK_END;
		return 0;
	}

	char u8ch[5];
	*tslSource_getUtf8Char(&lex->source, u8ch) = 0;
	tsl_charEscape(u8ch);
	if (lex->errbuf)
		TSL_SNPRINTF(lex->errbuf, lex->errbuf_sz, "Unexpected character '%s'", u8ch);
	return 1;
}
