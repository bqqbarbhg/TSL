#ifndef TSL_UTIL_H
#define TSL_UTIL_H

#ifdef _MSC_VER
#include <stdio.h>
#define TSL_SNPRINTF sprintf_s
#else
#include <stdio.h>
#define TSL_SNPRINTF snprintf
#endif

// Escapes non-drawable characters (includes 0-terminator)
// c: Buffer of at least 5 characters
void tsl_charEscape(char* c);

#endif
