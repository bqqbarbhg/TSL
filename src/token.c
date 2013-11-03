#include "token.h"
#include "source.h"
#include "util.h"
#include <stdio.h>
#include "memory.h"

#define TSL_LEXER_TMPSTRINGBUF_SIZE 64

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
	lex->col = 1;
	lex->tmpStrBufSize = TSL_LEXER_TMPSTRINGBUF_SIZE;
	lex->tmpStrBuf = (char*)tsl_malloc(lex->tmpStrBufSize);
	if (tslspeek(lex) == '\n')
		lex->line++;
}

void tslLexer_free(TSLLexer *lex)
{
	tslSource_free(&lex->source);
	free(lex->tmpStrBuf);
}

static void tslcharerror(TSLLexer *lex, const char* str)
{
	char u8ch[5];
	*tslSource_getUtf8Char(&lex->source, u8ch) = 0;
	tsl_charEscape(u8ch);
	if (lex->errbuf)
		TSL_SNPRINTF(lex->errbuf, lex->errbuf_sz, str, u8ch);
}

static char* tslUnicodeEscape(TSLLexer *lex, char *pc)
{
	char c;
	unsigned int codepoint = 0;
	for (int i = 0; i < 4; i++)
	{
		codepoint *= 0x10;
		c = tslsnext(lex);
		if (c >= '0' && c <= '9')
			codepoint += c - '0';
		else if (c >= 'A' && c <= 'F')
			codepoint += c - 'A' + 0xA;
		else if (c >= 'a' && c <= 'f')
			codepoint += c - 'a' + 0xA;
		else
		{
			lex->errType = TSLLEXERR_INVALID_HEX_DIGIT;
			tslcharerror(lex, "'%s' is not a valid hex digit");
			return 0;
		}
	}
	char utf8[5];
	char *u8end = tsl_pointToUtf8(utf8, codepoint);
	if (utf8 == u8end)
	{
		lex->errType = TSLLEXERR_INVALID_UNCIODE_ESCAPE;
		if (lex->errbuf)
			TSL_SNPRINTF(lex->errbuf, lex->errbuf_sz, "'\\u%04X' is not a valid unicode character", codepoint);
		return 0;
	}

	if (pc - lex->tmpStrBuf + (u8end - utf8) > lex->tmpStrBufSize)
	{
		size_t newsize = lex->tmpStrBufSize * 2;
		lex->tmpStrBuf = tsl_realloc(lex->tmpStrBuf, newsize);
		pc = lex->tmpStrBuf + lex->tmpStrBufSize;
		lex->tmpStrBufSize = newsize;
	}

	for (char *p = utf8; p != u8end; ++p)
	{
		*pc++ = *p;
	}

	return pc;
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

	if (c == '"')
	{
		char *pc = lex->tmpStrBuf, *pe = lex->tmpStrBufSize;
		c = tslsnext(lex);
		while (c != '"')
		{
			if (pc == pe)
			{
				size_t newsize = lex->tmpStrBufSize * 2;
				lex->tmpStrBuf = tsl_realloc(lex->tmpStrBuf, newsize);
				pc = lex->tmpStrBuf + lex->tmpStrBufSize;
				pe = lex->tmpStrBuf + newsize;
				lex->tmpStrBufSize = newsize;
			}

			if (c == '\\')
			{
				c = tslsnext(lex);
				switch (c)
				{
				case '"': *pc++ = '"'; break;
				case 'b': *pc++ = '\b'; break;
				case 'f': *pc++ = '\f'; break;
				case 'n': *pc++ = '\n'; break;
				case 'r': *pc++ = '\r'; break;
				case 't': *pc++ = '\t'; break;
				case '\\': *pc++ = '\\'; break;
				case 'u':
						{
							pc = tslUnicodeEscape(lex, pc);
							if (pc == 0) return 2;
							pe = lex->tmpStrBuf + lex->tmpStrBufSize;
						}
					break;
				default:
					lex->errType = TSLLEXERR_UNKNOWN_ESCAPE_SEQUENCE;
					tslcharerror(lex, "Unknown escape sequence '\\%s'");
					return 1;
				}
			}
			else
			{
				*pc++ = c;
			}
			c = tslsnext(lex);
		}
		tslsnext(lex);

		if (pc == pe)
		{
			size_t newsize = lex->tmpStrBufSize * 2;
			lex->tmpStrBuf = tsl_realloc(lex->tmpStrBuf, newsize);
			pc = lex->tmpStrBuf + lex->tmpStrBufSize;
			pe = lex->tmpStrBuf + newsize;
			lex->tmpStrBufSize = newsize;
		}
		*pc++ = 0;

		dst->type = TSLTOK_STRING;
		dst->val.string = tsl_memdup(lex->tmpStrBuf, pc - lex->tmpStrBuf);
		return 0;
	}

	lex->errType = TSLLEXERR_UNEXPECTED_CHARACTER;
	tslcharerror(lex, "Unexpected character '%s'");
	return 1;
}