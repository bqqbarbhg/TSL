#include "util.h"

void tsl_charEscape(char* c)
{
	if (*c < 32)
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
