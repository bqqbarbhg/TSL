#include "token.h"
#include "source.h"
#include "util.h"
#include <stdio.h>

int tslToken_get(TSLTokenStream *stream, TSLToken *dst)
{
	char c = tslSource_peek(&stream->source);

	if (c == 0)
	{
		dst->type = TSLTOK_END;
		return 0;
	}

	char u8ch[5];
	*tslSource_getUtf8Char(&stream->source, u8ch) = 0;
	if (stream->errbuf)
		TSL_SNPRINTF(stream->errbuf, stream->errbuf_sz, "Unexpected character '%s'", u8ch);
	return 1;
}
