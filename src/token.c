#include "token.h"
#include "source.h"
#include "util.h"
#include <stdio.h>

static char tslspeek(TSLSource *src)
{
	char c = tslSource_peek(src);
	while (c == '\r')
		c = tslSource_next(src);
	return c;
}

static char tslsnext(TSLSource *src)
{
	char c = tslSource_next(src);
	while (c == '\r')
		c = tslSource_next(src);
	return c;
}

int tslToken_get(TSLLexer *stream, TSLToken *dst)
{
	char c = tslspeek(&stream->source);

	for (;;)
	{
		while (c == ' ' || c == '\t' || c == '\n')
			c = tslsnext(&stream->source);
		if (c == '#')
		{
			// Skip line comment
			c = tslsnext(&stream->source);
			while (c != '\n' && c != 0)
				c = tslsnext(&stream->source);
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
	*tslSource_getUtf8Char(&stream->source, u8ch) = 0;
	tsl_charEscape(u8ch);
	if (stream->errbuf)
		TSL_SNPRINTF(stream->errbuf, stream->errbuf_sz, "Unexpected character '%s'", u8ch);
	return 1;
}
