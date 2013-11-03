#include "util.h"
#include "memory.h"
#include <string.h>

void tsl_charEscape(char* c)
{
	if (*(unsigned char*)c < 32)
	{
		switch (*c)
		{
		case '\a': c[0] = '\\'; c[1] = 'a'; c[2] = 0; break;
		case '\b': c[0] = '\\'; c[1] = 'b'; c[2] = 0; break;
		case '\f': c[0] = '\\'; c[1] = 'f'; c[2] = 0; break;
		case '\n': c[0] = '\\'; c[1] = 'n'; c[2] = 0; break;
		case '\r': c[0] = '\\'; c[1] = 'r'; c[2] = 0; break;
		case '\t': c[0] = '\\'; c[1] = 't'; c[2] = 0; break;
		case '\v': c[0] = '\\'; c[1] = 'v'; c[2] = 0; break;
		case '\0': c[0] = '\\'; c[1] = '0'; c[2] = 0; break;
		default: sprintf(c, "\\x%02x", *c); break;
		}
		
	}
}

char *tsl_pointToUtf8(char *ptr, unsigned int codepoint)
{
	if (codepoint < 0x0080)
	{
		*ptr = codepoint;
		return ptr + 1;
	}
	else if (codepoint < 0x0800)
	{
		*ptr++ = 0xC0 | codepoint >> 6 & 0x1F;
		*ptr++ = 0x80 | codepoint & 0x3F;
		return ptr;
	}
	else if (codepoint < 0x10000)
	{
		*ptr++ = 0xE0 | codepoint >> 12 & 0x1F;
		*ptr++ = 0x80 | codepoint >> 6 & 0x3F;
		*ptr++ = 0x80 | codepoint & 0x3F;
		return ptr;
	}
	else if (codepoint < 0x10FFFF)
	{
		*ptr++ = 0xF0 | codepoint >> 18 & 0x1F;
		*ptr++ = 0x80 | codepoint >> 12 & 0x3F;
		*ptr++ = 0x80 | codepoint >> 6 & 0x3F;
		*ptr++ = 0x80 | codepoint & 0x3F;
		return ptr;
	}
	else
		return ptr;
}

void *tsl_memdup(const void *buf, size_t size)
{
	void *p = tsl_malloc(size);
	memcpy(p, buf, size);
	return p;
}

char *tsl_strdup(const char *str)
{
	return (char*)tsl_memdup(str, strlen(str));
}